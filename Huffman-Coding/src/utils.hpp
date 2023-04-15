#pragma once

#include <unistd.h>

#include <cmath>
#include <bitset>
#include <string>
#include <vector>

constexpr auto INF_SIZET = std::numeric_limits<size_t>::max();

// TODO: d-ary
using DataType = std::basic_string<bool>;
using CharValue = std::bitset<8>;

void timer_start(std::string);
double timer_stop();

void timer_start_progress(std::string);
void timer_progress(double);
double timer_stop_progress();

template<typename T, typename U>
double calc_entropy(const std::vector<std::pair<T, U>>& freq);
