#include "huffman-tree.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

template<uint8_t logD, typename V>
HuffmanTree<logD, V>::HuffmanTree::
Node::Node(): end(false), freq(0), height(0), cls{} {}

template<uint8_t logD, typename V>
HuffmanTree<logD, V>::HuffmanTree::
Node::Node(size_t f, V v): end(true), freq(f), height(1), value(v) {}

template<uint8_t logD, typename V>
size_t
HuffmanTree<logD, V>::HuffmanTree::
newNode() {
    size_t idx = nodes.size();
    nodes.emplace_back();
    return idx;
}

template<uint8_t logD, typename V>
size_t
HuffmanTree<logD, V>::HuffmanTree::
newNode(size_t f, V v) {
    size_t idx = nodes.size();
    nodes.emplace_back(f, v);
    return idx;
}

template<uint8_t logD, typename V>
HuffmanTree<logD, V>::
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

template<uint8_t logD, typename V>
HuffmanTree<logD, V>::
HuffmanTree(size_t _b, Data& data):
    bits(_b), root(0), nodes(1)
{
    nodes.reserve(data.size() / bits * 2);
    parse(root, data);
}

template<uint8_t logD, typename V>
size_t
HuffmanTree<logD, V>::
height() const {
    return nodes[root].height;
}

template<uint8_t logD, typename V>
Data
HuffmanTree<logD, V>::
dump() {
    Data data;
    dump(root, data);
    return data;
}

template<uint8_t logD, typename V>
void
HuffmanTree<logD, V>::
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

template<uint8_t logD, typename V>
void
HuffmanTree<logD, V>::
parse(size_t idx, Data& data) {
    nodes[idx].end = data.readint(1);
    if (nodes[idx].end) {
        nodes[idx].value = data.readint(bits);
        nodes[idx].height = 1;
    } else {
        for (size_t i = 0; i < D and !data.eof(); i++) {
            auto nt = nodes[idx].cls[i] = newNode();
            parse(nt, data);
            nodes[idx].height = std::max(nodes[idx].height, nodes[nt].height + 1);
        }
    }
}

template<uint8_t logD, typename V>
void
HuffmanTree<logD, V>::
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
    if (0) {
        std::cerr << "dump table" << std::endl;
        std::vector<std::pair<V, DataType>> v(table.begin(), table.end());
        std::sort(v.begin(), v.end());
        for(auto [x, y]: v) {
            std::cerr << std::hex << x << std::dec << " -> ";
            for(auto z: y) std::cerr << z;
            std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }
}

template<uint8_t logD, typename V>
DataType
HuffmanTree<logD, V>::
encode(const V value) {
    return table.find(value)->second;
}

template<uint8_t logD, typename V>
V
HuffmanTree<logD, V>::
decode(DataSrc& src) {
    auto idx = root;
    while (!nodes[idx].end) {
        auto b =  src.readint(logD);
        idx = nodes[idx].cls[b];
    }
    return nodes[idx].value;
}

template class HuffmanTree<1, uint64_t>;
