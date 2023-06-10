#include "arithmetic-code.hpp"

Arithmetic::Arithmetic(std::vector<uint32_t>& _acc):
    accum(_acc), size(_acc.back()), L(0), U(MAX), scale3(0) {}

void Arithmetic::update(uint32_t idx) {
    auto vL = L.to_ullong(), vU = U.to_ullong();
    uint64_t d = vU - vL + 1;
    U = vL + d * accum[idx+1] / size - 1;
    L = vL + d * accum[idx] / size;
}

DataType Arithmetic::send(uint32_t idx) {
    update(idx);

    DataType bs{};
    while (true) {
        auto E3 = L[BITS-1] == 0 and L[BITS-2] == 1 and U[BITS-1] == 1 and U[BITS-2] == 0;
        if (L[BITS-1] != U[BITS-1] and !E3)
            break;
        auto b = (int)L[BITS-1];
        L <<= 1; L[0] = 0;
        U <<= 1; U[0] = 1;
        if (!E3) {
            bs.emplace_back(b);
            b ^= 1;
            for (int i = 0; i < scale3; i++)
                bs.emplace_back(b);
            scale3 = 0;
        } else {
            L[BITS-1] = L[BITS-1] ^ 1;
            U[BITS-1] = U[BITS-1] ^ 1;
            scale3++;
        }
    }

    return bs;
}

uint32_t Arithmetic::recv(DataSrc& src) {
    uint32_t code = (uint32_t)(
        ((T.to_ullong() - L.to_ullong() + 1) * size - 1)
        / (U.to_ullong() - L.to_ullong() + 1)
    );

    uint32_t idx = 0;
    while (accum.at(idx) <= code) idx++;
    idx--;

    update(idx);

    while (true) {
        auto E3 = L[BITS-1] == 0 and L[BITS-2] == 1 and U[BITS-1] == 1 and U[BITS-2] == 0;
        if (L[BITS-1] != U[BITS-1] and !E3)
            break;
        L <<= 1; L[0] = 0;
        U <<= 1; U[0] = 1;
        T <<= 1; T[0] = src.nextbit();
        if (E3) {
            L[BITS-1] = L[BITS-1] ^ 1;
            U[BITS-1] = U[BITS-1] ^ 1;
            T[BITS-1] = T[BITS-1] ^ 1;
        }
    }

    return idx;
}
