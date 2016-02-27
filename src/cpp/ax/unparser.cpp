#include <istream>

#include "../../hpp/ax/impl/unparser.hpp"

#include "../../hpp/ax/impl/functional.hpp"

namespace ax
{
    // NOTE: this is implemented in the pure functional style, so may not be fast enough for certain cases.
    std::string unparse_symbol(const symbol& symbol)
    {
        return
            match3(symbol,
            [&](VAL& atom) { return atom; },
            [&](VAL& quote) { return "`"_s + quote + "'"; },
            [&](VAL& symbols)
            {
                VAL& symbols_str = fold(symbols, ""_s, [](VAL& str, VAL& symbol)
                {
                    VAL& prestr = str.size() == 0 ? str : str + " ";
                    return prestr + unparse_symbol(symbol);
                });
                return "[" + symbols_str + "]";
            });
    }
}
