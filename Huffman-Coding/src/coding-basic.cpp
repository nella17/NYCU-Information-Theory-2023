#include "coding-basic.hpp"

#include "options.hpp"
#include "huffman-tree.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <unordered_map>

namespace coding {

Basic::Basic(): Base() {}

Basic::~Basic() {}

void Basic::encode(DataSrc& src, DataDst& dst) {
    std::unordered_map<uint64_t, size_t> map{};
    size_t total = 0;

    timer_start("calculate pmf");
    while (src.eof()) {
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

    auto entropy = calc_entropy(freq);
    std::cerr << "entropy: " << entropy << '\n'
        << "max compress rate: " << entropy / double(opts.bits) << '\n'
        << "charset size: " << freq.size() << '\n'
        << std::flush;
    if (opts.verbose) {
        std::cerr << "freq:";
        for (auto [c, v]: freq)
            std::cerr << ' ' << c;
        std::cerr << std::endl;
    }

    HuffmanTree ht(opts.bits, freq);
    std::cerr << "Tree Height: " << ht.height() << std::endl;
    dst.write(ht.dump());

    ht.buildtable();

    dst.writeint(32, total);

    timer_start("compress file");
    src.reset();
    size_t size = 0;
    size_t r = 0;
    for (size_t cnt = 0; src.eof(); cnt++) {
        uint64_t value = src.readint(opts.bits);
        auto code = ht.encode(value);
        size += code.size();
        dst.write(code);
        if (100 * cnt / total > r) {
            if (r++) std::cerr << "\b\b\b";
            std::cerr << std::setw(2) << r << '%' << std::flush;
        }
    }
    std::cerr << "\b\b\b";
    timer_stop();

   auto origsize = int64_t(total) * int64_t(opts.bits);
    std::cerr
        << "Original size: " << origsize << " bytes\n"
        << "Compressed size: " << size << " bytes\n"
        << "Compression rate: " << (double)(origsize - (int64_t)size) / (double)origsize << '\n'
        << std::flush;
}

void Basic::decode(DataSrc& /* src */, DataDst& /* dst */) {
}

}
