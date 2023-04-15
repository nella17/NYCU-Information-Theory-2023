#include "coding-basic.hpp"

#include "datasource.hpp"

#include <iostream>
#include <unordered_map>

namespace coding {

Basic::Basic(const Options& opt): Base(), bits(opt.bits) {}

std::string Basic::encode(DataSource& src) {
    std::unordered_map<uint64_t, size_t> freq{};
    size_t total = 0;

    while (src.empty()) {
        uint64_t value = src.readint(bits);
        freq[value] += 1;
        total += 1;
    }

    return "";
}

std::string Basic::decode(DataSource& src) {
    return "";
}

}
