#include "coding.hpp"

#include "coding-arithmetic-ppm.hpp"
#include "coding-arithmetic-fpm.hpp"

namespace coding {

std::unique_ptr<Base> make() {
    if (opts.type == ArithmeticPPM::TYPE) {
        auto func = std::make_unique<ArithmeticPPM>();
        return func;
    }

    if (opts.type == ArithmeticFPM::TYPE) {
        auto func = std::make_unique<ArithmeticFPM>();
        return func;
    }

    return nullptr;
}

}
