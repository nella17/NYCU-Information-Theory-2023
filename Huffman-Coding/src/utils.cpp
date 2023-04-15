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
