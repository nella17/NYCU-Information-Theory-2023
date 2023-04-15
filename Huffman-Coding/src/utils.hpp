#include <unistd.h>

#include <cmath>
#include <bitset>
#include <string>
#include <vector>

using DataType = std::basic_string<bool>;
using CharValue = std::bitset<8>;

void timer_start(std::string);
double timer_stop();

template<typename T, typename U>
double calc_entropy(const std::vector<std::pair<T, U>>& freq);
