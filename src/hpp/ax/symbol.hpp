#ifndef AX_SYMBOL_HPP
#define AX_SYMBOL_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "prelude.hpp"
#include "choice.hpp"

namespace ax
{
    // Stores any type of information in a normalized, symbolic format.
    SUM_TYPE3(symbol,
        std::string, atom,
        std::string, quote,
        std::vector<ax::symbol>, symbols);
}

#endif
