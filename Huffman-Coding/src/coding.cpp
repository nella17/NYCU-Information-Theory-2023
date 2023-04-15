#include "coding.hpp"

namespace coding {

std::unique_ptr<Base> make() {
    if (opts.type == Basic::TYPE) {
        auto basic = std::make_unique<Basic>();
        return basic;
    }

    return nullptr;
}

}
