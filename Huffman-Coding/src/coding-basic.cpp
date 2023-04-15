#include "coding-basic.hpp"

#include "options.hpp"
#include "huffman-tree.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_map>

namespace coding {

Basic::Basic(): Base() {}

void Basic::encode(DataSrc& src, DataDst& dst) {
    std::unordered_map<uint64_t, size_t> map{};
    size_t total = 0;

    timer_start("calculate pmf");
    while (src.empty()) {
        uint64_t value = src.readint(opts.bits);
        map[value] += 1;
        total += 1;
    }
    timer_stop();

    std::vector<std::pair<size_t, uint64_t>> freq;
    freq.reserve(map.size());
    for (auto [v, c]: map)
        freq.emplace_back(c, v);
    std::sort(freq.begin(), freq.end());

    if (opts.verbose) {
        auto entropy = calc_entropy(freq);
        std::cerr << "entropy: " << entropy << '\n'
            << "max compress rate: " << entropy / double(opts.bits) << '\n'
            << "charset size: " << freq.size() << '\n'
            << "freq:";
        for (auto [c, v]: freq)
            std::cerr << ' ' << c;
        std::cerr << std::endl;
    }

    HuffmanTree ht(freq);
    if (opts.verbose) {
        std::cerr << "Tree Height: " << ht.height() << std::endl;
    }
}

void Basic::decode(DataSrc& /* src */, DataDst& /* dst */) {
}

}
