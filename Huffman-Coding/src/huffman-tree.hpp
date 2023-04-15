#pragma once

#include <cstdint>
#include <array>
#include <vector>

template<size_t D = 2, typename V = uint64_t>
class HuffmanTree {
public:
    struct Node;
    using NodePtr = Node*;

    struct NodePtrCmp {
        bool operator()(NodePtr, NodePtr);
    };

    struct Node {
        bool end;
        size_t freq, height;
        union {
            V value;
            std::array<NodePtr, D> cls;
        };
        Node();
        Node(size_t, V);
    };

    HuffmanTree(const std::vector<std::pair<size_t, V>>&);
    size_t height() const;

private:
    NodePtr root;
};
