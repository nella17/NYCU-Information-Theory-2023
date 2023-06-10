#include "arithmetic-code.hpp"

Arithmetic::Arithmetic():
    L(0), U(MAX), scale3(0) {}

void Arithmetic::update(uint32_t cL, uint32_t cR, uint32_t size) {
    auto vL = L.to_ullong(), vU = U.to_ullong();
    uint64_t d = vU - vL + 1;
    U = vL + d * cR / size - 1;
    L = vL + d * cL / size;
}

DataType Arithmetic::send(const std::vector<uint32_t>& accum, uint32_t idx) {
    return send(accum[idx], accum[idx+1], accum.back());
}

DataType Arithmetic::send(uint32_t cL, uint32_t cR, uint32_t size) {
    update(cL, cR, size);

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

uint32_t Arithmetic::recv(DataSrc& src, const std::vector<uint32_t>& accum) {
    uint32_t code = (uint32_t)(
        ((T.to_ullong() - L.to_ullong() + 1) * accum.back() - 1)
        / (U.to_ullong() - L.to_ullong() + 1)
    );

    uint32_t idx = 0;
    while (accum.at(idx) <= code) idx++;
    idx--;

    recv(src, accum[idx], accum[idx+1], accum.back());

    return idx;
}

void Arithmetic::recv(DataSrc& src, uint32_t cL, uint32_t cR, uint32_t size) {
    update(cL, cR, size);

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
}
