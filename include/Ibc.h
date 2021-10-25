#ifndef INTUITIVE_BLOCKCHAIN_INCLUDED
#define INTUITIVE_BLOCKCHAIN_INCLUDED

#define _UNICODE
#define UNICODE

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#pragma message("-----------------------_DEBUG")
#else
#pragma message("-----------------------NOT _DEBUG")
#endif

#include <any>
#include <ctime>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace Ibc {

    // 明細1行に対応するクラス
    class Record {
    public:
        Record(int id, long amount);
        Record(std::time_t dateTime, int id, long amount);
        ~Record();

        std::string toString() const;
        std::time_t dateTime() const;
        int id() const;
        long amount() const;

    private:
        // 取引時刻
        std::time_t dateTime_;
        // 取引ID
        int id_;
        // 取引額
        long amount_;
    };

    // 取引記録のブロック
    class Block {
    public:
        Block(const std::vector<Record> &data, const size_t prev);
        ~Block();
        std::string toString() const;

    private:
        // このブロックが持つ取引明細
        std::vector<Record> data_;
        // 直前のブロックのハッシュ値
        size_t prev_;
        // 自身のハッシュ値
        size_t hash_;
    };

    // 取引記録を分散管理する店
    class Node {
    public:
        Node(int no, int shiftNo_, int totalNumberOfNodes, bool shift = false);
        ~Node();
        void start();

        void sendRecords(const std::vector<Record> &records, int no);

        // コピー禁止
        Node(const Node &) = delete;
        Node &operator=(const Node &) = delete;
        // ムーブ禁止
        Node(Node &&) = delete;
        Node &operator=(Node &&) = delete;

    private:
        void action();

        // 店番号
        int no_;
        // 当番の店番号
        int shiftNo_;
        // 全ての店の数
        int totalNumberOfNodes_;
        // 当番か否か
        bool shift_;
        // 取引記録
        std::vector<Block> ledger_;
        //std::vector<Record> temp_;
        // データ受信用変数
        std::any temp_;
        // この店が処理をするスレッド
        std::thread thread_;
    };

    // 各店を含むネットワーク
    class Network {
    public:
        static Network &instance();
        static Network &initialize(const std::vector<std::unique_ptr<Ibc::Node>> &nodes);

        ~Network();

        //void sendData(const std::vector<Record> &records, int no);
        void sendData(const std::any data, int no);

        // コピー禁止
        Network(const Network &) = delete;
        Network &operator=(const Network &) = delete;
        // ムーブ禁止
        Network(Network &&) = delete;
        Network &operator=(Network &&) = delete;

    private:
        Network();
        void init(const std::vector<std::unique_ptr<Ibc::Node>> &nodes);

        const std::vector<std::unique_ptr<Ibc::Node>> *pNodes_;
        bool isInitialized_;
        std::mutex mtx_;
    };

} // namespa

#endif // INTUITIVE_BLOCKCHAIN_INCLUDED
