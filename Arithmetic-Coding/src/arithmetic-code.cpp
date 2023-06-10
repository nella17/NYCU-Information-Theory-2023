#include "arithmetic-code.hpp"

Arithmetic::Arithmetic(std::vector<uint32_t>& _acc):
    accum(_acc), size(_acc.back()), L(0), U(MAX), scale3(0) {}

DataType Arithmetic::send(uint32_t symbol) {
    {
        auto vL = L.to_ullong(), vU = U.to_ullong();
        uint64_t d = vU - vL + 1;
        U = vL + d * accum[symbol+1] / size - 1;
        L = vL + d * accum[symbol] / size;
    }

    DataType bs{};
    while (true) {
        if (L[BITS-1] == U[BITS-1]) {
            auto b = (int)L[BITS-1];
            L <<= 1; L[0] = 0;
            U <<= 1; U[0] = 1;
            bs.emplace_back(b);
            b ^= 1;
            for (int i = 0; i < scale3; i++)
                bs.emplace_back(b);
            scale3 = 0;
        } else if (L[BITS-1] == 0 and L[BITS-2] == 1 and U[BITS-1] == 1 and U[BITS-2] == 0) {
            L <<= 1; L[0] = 0;
            U <<= 1; U[0] = 1;
            L[BITS-1] = L[BITS-1] ^ 1;
            U[BITS-1] = U[BITS-1] ^ 1;
            scale3++;
        } else {
            break;
        }
    }

    return bs;
}
