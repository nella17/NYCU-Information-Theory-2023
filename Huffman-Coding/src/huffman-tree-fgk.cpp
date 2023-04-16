#include "huffman-tree-fgk.hpp"

template<uint8_t logD, typename V>
HuffmanTreeFGK<logD, V>::HuffmanTreeFGK::
Node::Node(): leaf(false), freq(0), height(0), cls{} {}

template<uint8_t logD, typename V>
HuffmanTreeFGK<logD, V>::HuffmanTreeFGK::
Node::Node(V v): leaf(true), freq(1), height(1), value(v) {}

template<uint8_t logD, typename V>
size_t
HuffmanTreeFGK<logD, V>::
newNode() {
    size_t idx = nodes.size();
    nodes.emplace_back();
    return idx;
}


template<uint8_t logD, typename V>
size_t
HuffmanTreeFGK<logD, V>::
newNode(V v) {
    size_t idx = nodes.size();
    nodes.emplace_back(v);
    return idx;
}

template<uint8_t logD, typename V>
HuffmanTreeFGK<logD, V>::
HuffmanTreeFGK(size_t _b):
    bits(_b), root(0), NYT(0), nodes(1) {}

template<uint8_t logD, typename V>
size_t
HuffmanTreeFGK<logD, V>::
height() const {
    return nodes[root].height;
}

template<uint8_t logD, typename V>
DataType
HuffmanTreeFGK<logD, V>::
encode(const V) {
    // TODO
}

template<uint8_t logD, typename V>
V
HuffmanTreeFGK<logD, V>::
decode(DataSrc&) {
    // TODO
}

template<uint8_t logD, typename V>
void
HuffmanTreeFGK<logD, V>::
update(size_t) {
    // TODO
}

template class HuffmanTreeFGK<1, uint64_t>;
