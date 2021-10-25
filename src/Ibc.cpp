#include "Ibc.h"

#include <functional>
#include <iostream>
#include <random>
#include <sstream>

namespace Ibc {

    /**
     * Record
    */
    Record::Record(const int id, const long amount, const int no)
        : dateTime_{std::time(nullptr)},
          id_{id},
          amount_{amount},
          no_{no}
    {
    }

    Record::Record(const std::time_t dateTime, const int id, const long amount, const int no)
        : dateTime_{dateTime},
          id_{id},
          amount_{amount},
          no_{no}
    {
    }

    Record::~Record()
    {
        // noop
    }

    std::string Record::concatenate() const
    {
        std::ostringstream oss;
        oss << std::to_string(dateTime_)
            << std::to_string(id_)
            << std::to_string(amount_)
            << std::to_string(no_);
        return oss.str();
    }

    std::time_t Record::dateTime() const { return dateTime_; }
    int Record::id() const { return id_; }
    long Record::amount() const { return amount_; }
    int Record::no() const { return no_; }

    /**
     * Block
    */
    Block::Block(const std::vector<Record> &data, const size_t prev)
        : data_{data},
          prev_{prev}
    {
        std::ostringstream oss;
        for (const auto &e : data_) {
            oss << e.concatenate();
        }
        hash_ = std::hash<std::string>()(oss.str());
    }

    Block::~Block()
    {
        // noop
    }

    std::string Block::toString() const
    {
        std::ostringstream oss;
        for (const auto &e : data_) {
            oss << e.dateTime() << ","
                << e.id() << ","
                << e.amount() << ","
                << e.no() << std::endl;
        }
        oss << std::to_string(prev_) << std::endl
            << std::to_string(hash_) << std::endl;
        return oss.str();
    }

    size_t Block::hash() const
    {
        return hash_;
    }

    /**
     * Node
    */
    Node::Node(const int no, const int shiftNo, const int totalNumberOfNodes, const bool shift)
        : no_{no},
          shiftNo_{shiftNo},
          totalNumberOfNodes_{totalNumberOfNodes},
          shift_{shift}
    {
    }

    Node::~Node()
    {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    void Node::start()
    {
        thread_ = std::thread([this] { action(); });
    }

    void Node::action()
    {
        try {
            std::cout << "Node::action()" << std::endl;
            while (true) {
                if (Network::instance().getQuit())
                    return;

                // 明細作成(取引記録)
                std::vector<Record> records;
                for (int i = 0; i < 10; ++i) {
                    std::random_device rd;
                    std::mt19937 eng(rd());
                    // [100, 500)の範囲で実数を乱数生成する
                    std::uniform_int_distribution<> dist(100, 500);
                    // その時間(ミリ秒)スレッドを停止する
                    std::this_thread::sleep_for(std::chrono::milliseconds(dist(eng)));
                    records.push_back(Record{100 + i, 10000 + i, no_});
                }

                // 当番か否かで分岐
                if (shift_) {

                    { // Scoped Locking
                        // 各店から明細が全て送られるのを待つ
                        std::unique_lock<std::mutex> lk(mtx_);
                        cv_.wait(lk, [this] { return (temp_.size() == totalNumberOfNodes_ - 1); });
                        std::cout << "SHIFT No" << no_ << " temp_.size() : " << temp_.size() << std::endl;
                        // 送られてきたデータをRecordのvectorにコピーする
                        for (const auto &e : temp_) {
                            std::vector<Record> vec = std::any_cast<std::vector<Record>>(e);
                            for (const Record &r : vec) {
                                records.push_back(r);
                            }
                        }
                        // データクリア
                        temp_.clear();
                    }

                    // for (const Record &r : records) {
                    //     std::cout << r.concatenate() << std::endl;
                    // }
                    // std::cout << std::endl;

                    // ブロックを作成する
                    size_t hash = 0;
                    if (ledger_.size() == 0) {
                        // 初回のブロックであった場合は前回のハッシュ値としてダミーの値を入れることにする
                        hash = std::hash<std::string>()(Record{0, 0, 0}.concatenate());
                    }
                    else {
                        hash = ledger_.back().hash();
                    }
                    Block b{records, hash};

                    for (int i = 0; i < totalNumberOfNodes_; ++i) {
                        // 自身以外のNoの店に送信する
                        if (i != no_) {
                            Network::instance().sendData(b, i);
                        }
                    }
                    // ブロック追加
                    ledger_.push_back(b);
                }
                else {
                    // 明細送信処理
                    Network::instance().sendData(records, shiftNo_);

                    { // Scoped Locking
                        // ブロックが送られて来るのを待つ
                        std::unique_lock<std::mutex> lk(mtx_);
                        cv_.wait(lk, [this] { return temp_.size() == 1; });
                        Block data = std::any_cast<Block>(temp_.at(0));
                        ledger_.push_back(data);

                        std::cout << "No" << no_ << " ledger_.size() : " << ledger_.size() << std::endl;
                        // データクリア
                        temp_.clear();
                    }
                }

                // 当番を変更
                ++shiftNo_;
                if (shiftNo_ == totalNumberOfNodes_) {
                    shiftNo_ = 0;
                }
                // 自身のNoになった場合は当番となる
                if (shiftNo_ == no_)
                    shift_ = true;
                else
                    shift_ = false;
            }
        }
        catch (const std::exception &e) {
            std::cerr << "No" << no_ << " : " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "unexpected error." << std::endl;
        }
    }

    void Node::receiveData(const std::any data)
    {
        std::lock_guard<std::mutex> lk(mtx_);
        temp_.push_back(data);
        cv_.notify_one();
    }

    /**
     * Network
    */
    Network &Network::instance()
    {
        static Network *instance = new Network();
        return *instance;
    }

    Network &Network::initialize(const std::vector<std::unique_ptr<Ibc::Node>> &nodes)
    {
        Network &instance = Network::instance();
        instance.init(nodes);
        return instance;
    }

    Network::Network()
        : pNodes_{nullptr},
          isInitialized_{false},
          quit_{false}
    {
    }

    Network::~Network()
    {
        // noop
    }

    void Network::sendData(const std::any data, int no)
    {
        pNodes_->at(no)->receiveData(data);
    }

    void Network::setQuit()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        quit_ = true;
    }

    bool Network::getQuit()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        return quit_;
    }

    void Network::init(const std::vector<std::unique_ptr<Ibc::Node>> &nodes)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        if (!isInitialized_) {
            pNodes_ = &nodes;
            isInitialized_ = true;
        }
    }

} // namespace Ibc
