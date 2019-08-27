#ifndef IMPL_AX_STRING_HPP
#define IMPL_AX_STRING_HPP

#include <string>

#include "prelude.hpp"
#include "vector.hpp"

// Implemente std::string literal.
inline std::string operator ""_s(const char *str, std::size_t len)
{
    return std::string(str, len);
}

namespace ax
{
    // Split a string on a char delimiter.
    std::vector<std::string> split_string(const std::string& str, char delimiter);

    // Join a vector of string on a char delimiter.
    std::string join_strings(const std::vector<std::string>& strs, char delimiter);
}

#endif
