#pragma once

#include <memory>

#include "options.hpp"

#include "coding-base.hpp"

namespace coding {

    std::unique_ptr<Base> make();

}
