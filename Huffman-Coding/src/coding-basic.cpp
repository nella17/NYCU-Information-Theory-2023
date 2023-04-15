#include "coding-basic.hpp"

#include "options.hpp"

#include <iostream>
#include <unordered_map>

namespace coding {

Basic::Basic(): Base() {}

void Basic::encode(DataSrc& src, DataDst& dst) {
    std::unordered_map<uint64_t, size_t> freq{};
    size_t total = 0;

    timer_start("calculate pmf");
    while (src.empty()) {
        uint64_t value = src.readint(opts.bits);
        freq[value] += 1;
        total += 1;
    }
    timer_stop();

    if (opts.verbose) {
        auto entropy = calc_entropy(freq);
        std::cerr << "entropy: " << entropy << '\n'
            << "rate: " << entropy / double(opts.bits) << std::endl;
    }
}

void Basic::decode(DataSrc& /* src */, DataDst& /* dst */) {
}

}
