#include "coding-adaptive.hpp"

#include "options.hpp"
#include "huffman-tree-fgk.hpp"

namespace coding {

Adaptive::Adaptive(): Base() {}

Adaptive::~Adaptive() {}

size_t Adaptive::encode(DataSrc& src, DataDst& dst) {
    HuffmanTreeFGK ht(opts.bits);
}

size_t Adaptive::decode(DataSrc& src, DataDst& dst) {
    HuffmanTreeFGK ht(opts.bits);
}

}
