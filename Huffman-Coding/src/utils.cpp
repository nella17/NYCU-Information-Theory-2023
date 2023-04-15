#include "utils.hpp"

#include <iostream>
#include <chrono>

#include "options.hpp"

std::string readfile(int fd) {
    if (lseek(fd, 0, SEEK_SET) < 0) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }
    auto insize = (size_t)lseek(fd, 0, SEEK_END);
    auto indata = new char[insize];
    size_t readsize = 0;
    if (lseek(fd, 0, SEEK_SET) < 0) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }
    while (readsize < insize) {
        auto ret = read(fd, indata + readsize, insize - readsize);
        if (ret < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        readsize += (size_t)ret;
    }
    std::string ret{ indata, insize };
    delete [] indata;
    return ret;
}

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
