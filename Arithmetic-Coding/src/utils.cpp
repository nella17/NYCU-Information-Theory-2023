#include "utils.hpp"

#include <iomanip>
#include <chrono>

#include "options.hpp"

std::ostream& operator<<(std::ostream& os, DataType dt) {
    for (auto x: dt) os << x;
    return os;
}

auto start = std::chrono::steady_clock::now();
void timer_start(std::string s) {
    if (!opts.notime) std::cerr << "[timer] " << s << " ... " << std::flush;
    start = std::chrono::steady_clock::now();
}
double timer_stop() {
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    auto cnt = elapsed_seconds.count();
    if (!opts.notime) std::cerr << std::fixed << std::setprecision(2) << cnt << "s" << std::endl;
    return cnt;
}

double progress, last;
int size;
void timer_start_progress(std::string s) {
    timer_start(s);
    progress = last = 0;
    size = 4;
    if (!opts.notime) std::cerr << "0.0%" << std::flush;
}
void timer_progress(double p) {
    p *= 100;
    if (p - progress >= 0.05) {
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        auto cnt = elapsed_seconds.count();
        if (cnt - last >= 0.1) {
            last = cnt;
            progress = p;
            char buf[0x30];
            if (!opts.notime)
                std::cerr << std::string((size_t)size, '\b');
            size = snprintf(buf, sizeof(buf), "%.1f%% %.1fs", progress, cnt);
            if (!opts.notime)
                std::cerr << buf << std::flush;
        }
    }
}
double timer_stop_progress() {
    if (!opts.notime) std::cerr
        << std::string((size_t)size, '\b')
        << std::string((size_t)size, ' ')
        << std::string((size_t)size, '\b')
        << std::flush;
    return timer_stop();
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
