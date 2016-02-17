#ifndef IMPL_AX_HASH_HPP
#define IMPL_AX_HASH_HPP

#include <cstddef>
#include <numeric>
#include <functional>

#include "prelude.hpp"

namespace ax
{
    // Get the hash of a T value.
    template<typename T>
    std::size_t get_hash(const T& t)
    {
        return std::hash<T>()(t);
    }

    // Get the hash of a container in terms of its content.
    template<typename T, typename It>
    std::size_t get_hash_range(const It& begin, const It& end)
    {
        return std::accumulate(begin, end, 0_z, [](VAR acc, VAL& t) { return get_hash<T>(t) ^ acc; });
    }
}

#endif
