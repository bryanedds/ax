#ifndef IMPL_AX_SYMBOL_HPP
#define IMPL_AX_SYMBOL_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "prelude.hpp"
#include "either.hpp"

namespace ax
{
    // Stored any type of information in a normalized, symbolic format.
    SUM_TYPE(symbol, std::string, symbol_leaf, std::vector<symbol>, symbol_tree);
}

#endif
