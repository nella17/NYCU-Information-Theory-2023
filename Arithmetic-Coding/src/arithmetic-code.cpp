#include "arithmetic-code.hpp"

Arithmetic::Arithmetic(Data& _d):
    data(_d), L(0), U(MAX), scale3(0) {}

void Arithmetic::send(uint64_t cL, uint64_t cU, uint64_t tot) {
    uint64_t d = U - L + 1;
    U = L + d * cU / tot - 1;
    L = L + d * cL / tot;
    scaling();
}

void Arithmetic::scaling() {
    DataType bs{};
    while (true) {
        if (U <= MAX_HALF) {
            L = L * 2;
            U = U * 2;
            bs.emplace_back(0);
            for (int i = 0; i < scale3; i++)
                bs.emplace_back(1);
            scale3 = 0;
        } else if (MAX_HALF < L) {
            L = L * 2 - MAX;
            U = U * 2 - MAX;
            bs.emplace_back(1);
            for (int i = 0; i < scale3; i++)
                bs.emplace_back(0);
            scale3 = 0;
        } else if (MAX14 < L and L < MAX_HALF and U <= MAX34) {
            L = L * 2 - MAX_HALF;
            U = U * 2 - MAX_HALF;
            scale3++;
        } else {
            break;
        }
    }
    data.write(bs);
}
