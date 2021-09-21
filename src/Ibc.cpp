#include "Ibc.h"

#include <functional>
#include <iostream>
#include <sstream>

namespace Ibc {

    /**
     * Record
    */
    Record::Record(int id, long amount)
        : dateTime_{std::time(nullptr)},
          id_{id},
          amount_{amount}
    {
    }

    Record::Record(std::time_t dateTime, int id, long amount)
        : dateTime_{dateTime},
          id_{id},
          amount_{amount}
    {
    }

    Record::~Record()
    {
        // noop
    }

    std::string Record::toString() const
    {
        std::ostringstream oss;
        oss << std::to_string(dateTime_)
            << std::to_string(id_)
            << std::to_string(amount_);
        return oss.str();
    }

    std::time_t Record::dateTime() const { return dateTime_; }
    int Record::id() const { return id_; }
    long Record::amount() const { return amount_; }

    /**
     * Block
    */
    Block::Block(const std::vector<Record> &data, const size_t prev)
        : data_{data},
          prev_{prev}
    {
        std::ostringstream oss;
        for (const auto &e : data_) {
            oss << e.toString();
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
                << e.amount() << std::endl;
        }
        oss << std::to_string(prev_) << std::endl
            << std::to_string(hash_) << std::endl;
        return oss.str();
    }

    /**
     * Node
    */
    Node::Node(int no, int totalNumberOfNodes, bool shift)
        : no_{no},
          totalNumberOfNodes_{totalNumberOfNodes},
          shift_{shift}
    {
    }

    Node::~Node()
    {
        if (thread_.joinable())
            thread_.join();
    }

    void Node::start()
    {
        thread_ = std::thread([this] { action(); });
    }

    void Node::action()
    {
        std::cout << "Node::action()" << std::endl;
    }

} // namespace Ibc
