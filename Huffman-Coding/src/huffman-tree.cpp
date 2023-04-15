#include "huffman-tree.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

template<uint8_t D, typename V>
HuffmanTree<D, V>::HuffmanTree::
Node::Node(): end(false), freq(0), height(0), cls{} {}

template<uint8_t D, typename V>
HuffmanTree<D, V>::HuffmanTree::
Node::Node(size_t f, V v): end(true), freq(f), height(1), value(v) {}

template<uint8_t D, typename V>
size_t
HuffmanTree<D, V>::HuffmanTree::
newNode() {
    size_t idx = nodes.size();
    nodes.emplace_back();
    return idx;
}

template<uint8_t D, typename V>
size_t
HuffmanTree<D, V>::HuffmanTree::
newNode(size_t f, V v) {
    size_t idx = nodes.size();
    nodes.emplace_back(f, v);
    return idx;
}

template<uint8_t D, typename V>
HuffmanTree<D, V>::
HuffmanTree(size_t _b, const std::vector<std::pair<size_t, V>>& freq):
    bits(_b), root(0), nodes(1)
{
    nodes.reserve(freq.size() * 2);
    std::vector<size_t> idxs{};
    idxs.reserve(freq.size());
    for (const auto& [c, v]: freq)
        idxs.emplace_back(newNode(c, v));
    auto cmp = [&](size_t a, size_t b) {
        if (nodes[a].freq != nodes[b].freq)
            return nodes[a].freq > nodes[b].freq;
        return a > b;
    };
    std::priority_queue<size_t, std::vector<size_t>, decltype(cmp)> pq(idxs.begin(), idxs.end(), cmp);
    while (true) {
        auto next = pq.size() > D ? newNode() : root;
        for (size_t i = 0; i < D and !pq.empty(); i++) {
            auto idx = pq.top(); pq.pop();
            nodes[next].freq += nodes[idx].freq;
            nodes[next].height = std::max(nodes[next].height, nodes[idx].height + 1);
            nodes[next].cls[i] = idx;
        }
        if (pq.empty()) break;
        pq.emplace(next);
    }
}

template<uint8_t D, typename V>
size_t
HuffmanTree<D, V>::
height() const {
    return nodes[root].height;
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
dump(size_t idx, Data& data) {
    data.writeint(1, nodes[idx].end);
    if (nodes[idx].end) {
        data.writeint(bits, nodes[idx].value);
    } else {
        for (size_t i = 0; i < D; i++) {
            auto nt = nodes[idx].cls[i];
            if (nt) dump(nt, data);
        }
    }
}

template<uint8_t D, typename V>
void
HuffmanTree<D, V>::
buildtable() {
    table.clear();
    DataType dt{};
    std::function<void(size_t)> dfs;
    dfs = [&](size_t idx) {
        if (nodes[idx].end) {
            table.emplace(nodes[idx].value, dt);
        } else {
            for (size_t i = 0; i < D; i++) {
                auto nt = nodes[idx].cls[i];
                if (nt) {
                    dt.push_back(i);
                    dfs(nt);
                    dt.pop_back();
                }
            }
        }
    };
    dfs(root);
}

template<uint8_t D, typename V>
DataType
HuffmanTree<D, V>::
encode(const V value) {
    return table.find(value)->second;
}

template class HuffmanTree<2, uint64_t>;
