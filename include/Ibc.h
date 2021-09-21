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

#include <ctime>
#include <string>
#include <thread>
#include <vector>

namespace Ibc {

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
        std::time_t dateTime_;
        int id_;
        long amount_;
    };

    class Block {
    public:
        Block(const std::vector<Record> &data, const size_t prev);
        ~Block();
        std::string toString() const;

    private:
        std::vector<Record> data_;
        size_t prev_;
        size_t hash_;
    };

    class Node {
    public:
        Node(int no, int totalNumberOfNodes, bool shift = false);
        ~Node();
        void start();

        // コピー禁止
        Node(const Node &) = delete;
        Node &operator=(const Node &) = delete;
        // ムーブ禁止
        Node(Node &&) = delete;
        Node &operator=(Node &&) = delete;

    private:
        void action();

        int no_;
        int totalNumberOfNodes_;
        bool shift_;
        std::vector<Block> ledger_;
        std::vector<Record> temp_;
        std::thread thread_;
    };

} // namespace Ibc

#endif // INTUITIVE_BLOCKCHAIN_INCLUDED
