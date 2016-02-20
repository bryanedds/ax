#ifndef IMPL_AX_SYMBOL_HPP
#define IMPL_AX_SYMBOL_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "prelude.hpp"
#include "either.hpp"

namespace ax
{
    // Stores any type of information in a normalized, symbolic format.
    SUM_TYPE(symbol, std::string, atom, std::vector<symbol>, symbols);
}

#endif
