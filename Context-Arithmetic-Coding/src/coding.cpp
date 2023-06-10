#include "coding.hpp"

#include "coding-basic.hpp"
#include "coding-adaptive.hpp"

namespace coding {

std::unique_ptr<Base> make() {
    if (opts.type == Basic::TYPE) {
        auto basic = std::make_unique<Basic>();
        return basic;
    }

    if (opts.type == Adaptive::TYPE) {
        auto adaptive = std::make_unique<Adaptive>();
        return adaptive;
    }

    return nullptr;
}

}
