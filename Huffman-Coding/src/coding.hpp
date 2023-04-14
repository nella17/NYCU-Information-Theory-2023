#pragma once

#include <memory>

#include "options.hpp"

#include "coding-base.hpp"
#include "coding-basic.hpp"

namespace coding {

inline std::unique_ptr<Base> make(Options opt) {
    if (opt.type == Basic::TYPE) {
        auto basic = std::make_unique<Basic>();
        return basic;
    }

    return nullptr;
}

}
