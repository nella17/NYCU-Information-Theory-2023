#pragma once

#include "utils.hpp"
#include "data.hpp"

#include <cstdint>
#include <array>
#include <vector>
#include <unordered_map>

template<uint8_t D = 2, typename V = uint64_t>
class HuffmanTree {
public:
    struct Node {
        bool end;
        size_t freq, height;
        union {
            V value;
            // TODO: NodePtr -> size_t
            std::array<size_t, D> cls;
        };
        Node();
        Node(size_t, V);
    };

    HuffmanTree(size_t, const std::vector<std::pair<size_t, V>>&);
    size_t height() const;

    Data dump();
    void dump(size_t, Data&);

    void buildtable();
    DataType encode(const V);

    const size_t bits;
private:
    size_t newNode();
    size_t newNode(size_t, V);

    std::unordered_map<V, DataType> table;
    size_t root;
    std::vector<Node> nodes;
};
