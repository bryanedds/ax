#ifndef AX_STRING_HPP
#define AX_STRING_HPP

#include <string>
#include <vector>

#include "prelude.hpp"

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

    // Convert any stream compatible value to a std::string.
    // Here only because GCC can't seem to locate fucking std::to_string().
    std::string to_string(int32_t value);

    // Convert any stream compatible value to a std::string.
    // Here only because GCC can't seem to locate fucking std::to_string().
    std::string to_string(int64_t value);

    // Convert any stream compatible value to a std::string.
    // Here only because GCC can't seem to locate fucking std::to_string().
    std::string to_string(float value);

    // Convert any stream compatible value to a std::string.
    // Here only because GCC can't seem to locate fucking std::to_string().
    std::string to_string(double value);

    // Convert any stream compatible value to a std::string.
    // Here only because GCC can't seem to locate fucking std::to_string().
    std::string to_string(std::size_t value);
}

#endif
