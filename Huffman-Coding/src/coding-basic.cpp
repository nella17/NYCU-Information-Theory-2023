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

size_t Basic::encode(DataSrc& src, DataDst& dst) {
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
    if (opts.verbose) {
        std::cerr
            << "Entropy:                " << entropy << '\n'
            << "Entropy (per byte):     " << entropy / ((double)opts.bits / 8) << '\n'
            << "Max compression ratio:  "
                << std::setw(5) << std::setprecision(2) << std::fixed
                << 100 * ((double)opts.bits - entropy) / double(opts.bits) << "%\n"
            << "Charset size:           " << freq.size() << std::endl;
    }
    if (opts.pmf) {
        std::cerr << "Freq: ";
        for (auto [c, v]: freq)
            std::cerr << ' ' << c;
        std::cerr << std::endl;
    }

    timer_start("build huffman tree & table");
    HuffmanTree ht(opts.bits, freq);
    auto treedata = ht.dump();
    ht.buildtable();
    timer_stop();
    if (opts.verbose) {
        std::cerr
            << "Tree Height:            " << ht.height() << '\n'
            << "Tree size:              " << treedata.size() << '\n'
            << std::flush;
    }

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
    dst.write(opts.split == INF_SIZET);
    timer_stop();

    // for (auto x: data)
    //     std::cerr << x;
    // std::cerr << std::endl;

    auto compsize = (data.size() + 7) / 8;
    auto total_compsize = 32 / 8 + treedata.size() / 8 + 32 / 8 + compsize;
    if (opts.verbose) {
        std::cerr
            << "Original size    (split):   " << origsize << " bytes\n"
            << "Compressed size  (split):   " << compsize << " bytes (" << data.size() << " bits)\n"
            << "Compression ratio (split):  "
                << std::setw(5) << std::setprecision(2) << std::fixed
                << 100 * (double)((int64_t)origsize - (int64_t)compsize) / (double)origsize << "%\n"
            << std::flush;
    }

    return total_compsize;
}

size_t Basic::decode(DataSrc& src, DataDst& dst) {
    size_t treesize = src.readint(32);
    auto treedata = src.readdata(treesize);
    size_t origsize = src.readint(32);
    size_t total = origsize / (opts.bits / 8);
    size_t datasize = src.remain();
    auto compsize = datasize / 8;

    if (!origsize or !compsize) return 0;

    if (opts.verbose) {
        std::cerr
            << "Tree size:          " << treedata.size() << '\n'
            << "Original size:      " << origsize << " bytes\n"
            << "Compressed size:    " << compsize << " bytes (" << datasize << " bits)\n"
            << std::flush;
    }

    timer_start("restore huffman tree");
    HuffmanTree ht(opts.bits, treedata);
    ht.buildtable();
    timer_stop();
    if (opts.verbose) {
        std::cerr
            << "Tree Height: " << ht.height() << '\n'
            << std::flush;
    }

    timer_start_progress("decompress file");
    Data data;
    for (size_t cnt = 0; data.size() / 8 < origsize and !src.eof(); cnt++) {
        auto value = ht.decode(src);
        // std::cerr << " -> " << std::hex << value << std::endl;
        data.writeint(opts.bits, value);
        timer_progress(double(cnt) / double(total));
    }
    src.back(data.size() - origsize * 8);
    data.resize(origsize * 8);
    timer_stop_progress();

    timer_start("write file");
    dst.write(data);
    dst.write(opts.split == INF_SIZET);
    timer_stop();

    auto dsize = data.size() / 8;
    if (opts.verbose) {
        std::cerr
            << "Decompressed size:  " << dsize << " bytes\n"
            << std::flush;
    }

    return dsize;
}

}
