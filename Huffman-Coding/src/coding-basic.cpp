#include "coding-basic.hpp"

#include "options.hpp"
#include "huffman-tree.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_map>

namespace coding {

Basic::Basic(): Base() {}

Basic::~Basic() {}

void Basic::encode(DataSrc& src, DataDst& dst) {
    std::unordered_map<uint64_t, size_t> map{};
    size_t total = src.size() / opts.bits;

    timer_start_progress("calculate pmf");
    for (size_t cnt = 0; src.eof(); cnt++) {
        uint64_t value = src.readint(opts.bits);
        map[value] += 1;
        timer_progress(double(cnt) / double(total));
    }
    timer_stop_progress();

    std::vector<std::pair<size_t, uint64_t>> freq;
    freq.reserve(map.size());
    for (auto [v, c]: map)
        freq.emplace_back(c, v);
    std::sort(freq.begin(), freq.end());

    auto entropy = calc_entropy(freq);
    std::cerr << "Entropy: " << entropy << '\n'
        << "Max compress rate: " << (entropy - (double)opts.bits) / double(opts.bits) << '\n'
        << "Charset size: " << freq.size() << '\n'
        << std::flush;
    if (opts.verbose) {
        std::cerr << "Freq:";
        for (auto [c, v]: freq)
            std::cerr << ' ' << c;
        std::cerr << std::endl;
    }

    HuffmanTree ht(opts.bits, freq);
    std::cerr << "Tree Height: " << ht.height() << std::endl;
    dst.write(ht.dump());

    ht.buildtable();

    timer_start_progress("compress file");
    src.reset();
    DataType encode;
    for (size_t cnt = 0; src.eof(); cnt++) {
        uint64_t value = src.readint(opts.bits);
        auto code = ht.encode(value);
        encode.insert(
            encode.end(),
            code.begin(),
            code.end()
        );
        timer_progress(double(cnt) / double(total));
    }
    timer_stop_progress();

    dst.writeint(32, total);
    dst.write(encode);

    auto origsize = int64_t(total) * int64_t(opts.bits);
    auto size = encode.size();
    std::cerr
        << "Original size: " << origsize << " bytes\n"
        << "Compressed size: " << size << " bytes\n"
        << "Compression rate: " << (double)(origsize - (int64_t)size) / (double)origsize << '\n'
        << std::flush;
}

void Basic::decode(DataSrc& /* src */, DataDst& /* dst */) {
}

}
