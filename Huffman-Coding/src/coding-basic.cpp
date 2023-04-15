#include "coding-basic.hpp"

#include "options.hpp"

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
            << "rate: " << entropy / double(opts.bits) << '\n'
            << "freq:";
        for (auto [c, v]: freq)
            std::cerr << ' ' << c;
        std::cerr << std::endl;
    }
}

void Basic::decode(DataSrc& /* src */, DataDst& /* dst */) {
}

}
