#include <unistd.h>

#include <bitset>
#include <string>
#include <vector>

using DataType = std::basic_string<bool>;
using CharValue = std::bitset<8>;

std::string readfile(int);