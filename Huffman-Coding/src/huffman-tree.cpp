#include "huffman-tree.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

template<uint8_t D, typename V>
bool
HuffmanTree<D, V>::HuffmanTree::
NodePtrCmp::operator()(NodePtr a, NodePtr b) {
    if (a->freq != b->freq)
        return a->freq > b->freq;
    return a < b;
}

template<uint8_t D, typename V>
HuffmanTree<D, V>::HuffmanTree::
Node::Node(): end(false), freq(0), height(0), cls{} {}

template<uint8_t D, typename V>
HuffmanTree<D, V>::HuffmanTree::
Node::Node(size_t f, V v): end(true), freq(f), height(1), value(v) {}

template<uint8_t D, typename V>
HuffmanTree<D, V>::
HuffmanTree(size_t b, const std::vector<std::pair<size_t, V>>& freq):
    bits(b)
{
    timer_start("build HuffmanTree");
    std::priority_queue<NodePtr, std::vector<NodePtr>, NodePtrCmp> pq{};
    for (const auto& [c, v]: freq)
        pq.emplace(new Node(c, v));
    while (pq.size() > 1) {
        auto n = new Node();
        for (size_t i = 0; !pq.empty() and i < D; i++) {
            auto node = pq.top(); pq.pop();
            n->freq += node->freq;
            n->height = std::max(n->height, node->height + 1);
            n->cls[i] = node;
        }
        pq.emplace(n);
    }
    root = pq.top(); pq.pop();
    timer_stop();
}

template<uint8_t D, typename V>
size_t
HuffmanTree<D, V>::
height() const {
    return root->height;
}

template<uint8_t D, typename V>
Data
HuffmanTree<D, V>::
dump() {
    Data data;
    dump(root, data);
    return data;
}

template<uint8_t D, typename V>
void
HuffmanTree<D, V>::
dump(NodePtr node, Data& data) {
    data.writeint(1, node->end);
    if (node->end) {
        data.writeint(bits, node->value);
    } else {
        for (size_t i = 0; i < D; i++) {
            auto nt = node->cls[i];
            if (nt) dump(nt, data);
        }
    }
}

template<uint8_t D, typename V>
void
HuffmanTree<D, V>::
buildtable() {
    table.clear();
    timer_start("build table");
    DataType dt{};
    std::function<void(NodePtr)> dfs;
    dfs = [&](NodePtr it) {
        if (it->end) {
            table.emplace(it->value, dt);
        } else {
            for (size_t i = 0; i < D; i++) {
                auto nt = it->cls[i];
                if (nt) {
                    dt.push_back(i);
                    dfs(nt);
                    dt.pop_back();
                }
            }
        }
    };
    dfs(root);
    timer_stop();
}

template<uint8_t D, typename V>
DataType
HuffmanTree<D, V>::
encode(const V value) {
    if (table.empty()) buildtable();
    auto it = table.find(value);
    return it != table.end() ? it->second : DataType();
}

template class HuffmanTree<2, uint64_t>;
