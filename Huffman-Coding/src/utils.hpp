#include <unistd.h>

#include <cmath>
#include <bitset>
#include <string>
#include <vector>
#include <unordered_map>

using DataType = std::basic_string<bool>;
using CharValue = std::bitset<8>;

std::string readfile(int);

void timer_start(std::string);
double timer_stop();

#ifndef ENTROPY_CALC
#define ENTROPY_CALC
template<typename T, typename U>
inline double calc_entropy(const std::unordered_map<T, U>& map) {
    U total = 0;
    for (const auto [x, y]: map)
        total += y;
    double entropy = 0;
    for (const auto [x, y]: map) {
        // auto p = double(y) / double(total);
        // entropy -= p * std::log2(p);
        entropy += double(y) / double(total) * (std::log2(total) - std::log2(y));
    }
    return entropy;
}
#endif
