#pragma once

#include <memory>

#include "options.hpp"

#include "coding-base.hpp"
#include "coding-basic.hpp"

namespace coding {

    std::unique_ptr<Base> make();

}
