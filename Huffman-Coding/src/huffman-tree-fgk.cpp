#include "huffman-tree-fgk.hpp"

template<uint8_t logD, typename V, V NYTvalue>
HuffmanTreeFGK<logD, V, NYTvalue>::HuffmanTreeFGK::
Node::Node(): leaf(false), freq(0), height(0), cls{} {}

template<uint8_t logD, typename V, V NYTvalue>
HuffmanTreeFGK<logD, V, NYTvalue>::HuffmanTreeFGK::
Node::Node(V v): leaf(true), freq(1), height(1), value(v) {}

template<uint8_t logD, typename V, V NYTvalue>
size_t
HuffmanTreeFGK<logD, V, NYTvalue>::HuffmanTreeFGK::
newNode() {
    size_t idx = nodes.size();
    nodes.emplace_back();
    return idx;
}

template<uint8_t logD, typename V, V NYTvalue>
size_t
HuffmanTreeFGK<logD, V, NYTvalue>::HuffmanTreeFGK::
newNode(V v) {
    size_t idx = nodes.size();
    nodes.emplace_back(v);
    return idx;
}

template<uint8_t logD, typename V, V NYTvalue>
HuffmanTreeFGK<logD, V, NYTvalue>::
HuffmanTreeFGK(size_t _b):
    bits(_b), root(0), NYT(0), nodes(1) {}

template<uint8_t logD, typename V, V NYTvalue>
size_t
HuffmanTreeFGK<logD, V, NYTvalue>::
height() const {
    return nodes[root].height;
}

template<uint8_t logD, typename V, V NYTvalue>
DataType
HuffmanTreeFGK<logD, V, NYTvalue>::
encode(const V) {
    // TODO
}

template<uint8_t logD, typename V, V NYTvalue>
V
HuffmanTreeFGK<logD, V, NYTvalue>::
decode(DataSrc&) {
    // TODO
}

template<uint8_t logD, typename V, V NYTvalue>
void
HuffmanTreeFGK<logD, V, NYTvalue>::
update(size_t) {
    // TODO
}

template class HuffmanTreeFGK<1, uint64_t, (uint64_t)-1>;
