#ifndef ax_symbol_hpp
#define ax_symbol_hpp

#include <cstddef>
#include <string>
#include <vector>

#include "prelude.hpp"
#include "string.hpp"
#include "either.hpp"

namespace ax
{
    // Stored any type of information in a normalized, symbolic format.
    sum_type(symbol, std::string, symbol_leaf, std::vector<symbol>, symbol_tree);
}

#endif
