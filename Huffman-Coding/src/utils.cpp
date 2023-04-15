#include "utils.hpp"

#include <iostream>
#include <chrono>

#include "options.hpp"

auto start = std::chrono::steady_clock::now();
void timer_start(std::string s) {
    if (opts.verbose)
        std::cerr << "[timer] " << s << " ... " << std::flush;
    start = std::chrono::steady_clock::now();
}
double timer_stop() {
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    auto cnt = elapsed_seconds.count();
    if (opts.verbose)
        std::cerr << cnt << "s" << std::endl;
    return cnt;
}

template<typename T, typename U>
double calc_entropy(const std::vector<std::pair<T, U>>& freq) {
    T total = 0;
    for (const auto& [c, v]: freq)
        total += c;
    double entropy = 0;
    for (const auto& [c, v]: freq) {
        // auto p = double(c) / double(total);
        // entropy -= p * std::log2(p);
        entropy += double(c) / double(total) * (std::log2(total) - std::log2(c));
    }
    return entropy;
}

template double calc_entropy(const std::vector<std::pair<size_t, uint64_t>>&);
