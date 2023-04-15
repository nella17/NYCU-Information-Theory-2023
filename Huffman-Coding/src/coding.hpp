#pragma once

#include <memory>

#include "options.hpp"

#include "coding-base.hpp"
#include "coding-basic.hpp"

namespace coding {

#ifndef CODING_MAKE
#define CODING_MAKE
inline std::unique_ptr<Base> make() {
    if (opts.type == Basic::TYPE) {
        auto basic = std::make_unique<Basic>();
        return basic;
    }

    return nullptr;
}
#endif

}
