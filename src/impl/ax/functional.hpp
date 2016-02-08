#ifndef IMPL_AX_FUNCTIONAL_HPP
#define IMPL_AX_FUNCTIONAL_HPP

#include <cstddef>
#include <vector>

#include "prelude.hpp"

template<typename U, typename T, typename F>
std::vector<U> map(const std::vector<T>& source, const F& mapper)
{
    std::vector<U> mapped{};
    for (val& elem : source)
        mapped.emplace_back(mapper(elem));
    return mapped;
}

template<typename T, typename F>
std::vector<T> filter(const std::vector<T>& source, const F& predicate)
{
    std::vector<T> filtered{};
    for (val& elem : source)
        if (predicate(elem))
            filtered.emplace_back(elem);
    return filtered;
}

template<typename T, typename S, typename F>
S fold(const std::vector<T>& source, const S& seed, const F& folder)
{
    S state(seed);
    for (val& elem : source)
        state = folder(state, elem);
    return state;
}

template<typename Cr>
std::vector<typename Cr::value_type> to_vector(const Cr& source)
{
    constrain_as_container(Cr);
    return std::vector<typename Cr::value_type>(source.cbegin(), source.cend());
}

#endif
