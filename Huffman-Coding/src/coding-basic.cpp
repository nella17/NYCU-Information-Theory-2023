#include "coding-basic.hpp"

#include "options.hpp"
#include "huffman-tree.hpp"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <unordered_map>

namespace coding {

Basic::Basic(): Base() {}

Basic::~Basic() {}

void Basic::encode(DataSrc& src, DataDst& dst) {
    std::unordered_map<uint64_t, size_t> map{};
    size_t origsize = src.size() / 8;
    size_t total = origsize / (opts.bits / 8);

    timer_start_progress("calculate pmf");
    for (size_t cnt = 0; !src.eof(); cnt++) {
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
        << "Max compress rate: "
            << std::setw(5) << std::setprecision(2) << std::fixed
            << 100 * ((double)opts.bits - entropy) / double(opts.bits) << "%\n"
        << "Charset size: " << freq.size() << '\n'
        << std::flush;
    if (opts.verbose) {
        std::cerr << "Freq:";
        for (auto [c, v]: freq)
            std::cerr << ' ' << c;
        std::cerr << std::endl;
    }

    timer_start("build huffman tree & table");
    HuffmanTree ht(opts.bits, freq);
    auto treedata = ht.dump();
    ht.buildtable();
    timer_stop();
    std::cerr
        << "Tree Height: " << ht.height() << '\n'
        << "Tree size: " << treedata.size() << '\n'
        << std::flush;

    timer_start_progress("compress file");
    src.reset();
    DataType data;
    for (size_t cnt = 0; !src.eof(); cnt++) {
        uint64_t value = src.readint(opts.bits);
        auto code = ht.encode(value);
        data.insert(
            data.end(),
            code.begin(),
            code.end()
        );
        timer_progress(double(cnt) / double(total));
    }
    timer_stop_progress();

    timer_start("write file");
    dst.writeint(32, treedata.size());
    dst.write(treedata);
    dst.writeint(32, origsize);
    dst.write(data);
    dst.write(true);
    timer_stop();

    auto size = data.size() / int64_t(8);
    std::cerr
        << "Original size: " << origsize << " bytes\n"
        << "Compressed size: " << size << " bytes\n"
        << "Compression rate: "
            << std::setw(5) << std::setprecision(2) << std::fixed
            << 100 * (double)(origsize - size) / (double)origsize << "%\n"
        << std::flush;
}

void Basic::decode(DataSrc& src, DataDst& dst) {
    size_t treesize = src.readint(32);
    auto treedata = src.readdata(treesize);
    size_t origsize = src.readint(32);
    size_t total = origsize / (opts.bits / 8);

    std::cerr
        << "Tree size: " << treedata.size() << '\n'
        << "Original size: " << origsize << " bytes\n"
        << std::flush;
}

}
