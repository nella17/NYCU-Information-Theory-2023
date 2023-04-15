#include "huffman-tree.hpp"

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

#include "options.hpp"
#include "utils.hpp"

template<size_t D, typename V>
bool
HuffmanTree<D, V>::HuffmanTree::
NodePtrCmp::operator()(NodePtr a, NodePtr b) {
    if (a->freq != b->freq)
        return a->freq > b->freq;
    return a < b;
}

template<size_t D, typename V>
HuffmanTree<D, V>::HuffmanTree::
Node::Node(): end(false), freq(0), height(0) {}

template<size_t D, typename V>
HuffmanTree<D, V>::HuffmanTree::
Node::Node(size_t f, V v): end(true), freq(f), height(1), value(v) {}

template<size_t D, typename V>
HuffmanTree<D, V>::
HuffmanTree(const std::vector<std::pair<size_t, V>>& freq) {
    timer_start("build HuffmanTree");
    std::priority_queue<NodePtr, std::vector<NodePtr>, NodePtrCmp> pq{};
    for (const auto& [c, v]: freq)
        pq.emplace(new Node(c, v));
    if (opts.verbose) std::cerr << pq.size() << std::endl;
    while (pq.size() > 1) {
        auto n = new Node();
        for (size_t i = 0; !pq.empty() and i < D; i++) {
            auto node = pq.top(); pq.pop();
            n->freq += node->freq;
            n->height = std::max(n->height, node->height + 1);
            n->cls[i] = node;
        }
        pq.emplace(n);
        if (opts.verbose) std::cerr << pq.size() << std::endl;
    }
    root = pq.top(); pq.pop();
    timer_stop();
}

template<size_t D, typename V>
size_t
HuffmanTree<D, V>::
height() const {
    return root->height;
}

template class HuffmanTree<2, uint64_t>;
