#ifndef IMPL_AX_FUNCTIONAL_HPP
#define IMPL_AX_FUNCTIONAL_HPP

#include <cstddef>
#include <vector>

#include "prelude.hpp"

namespace ax
{
    template<typename U, typename T, typename F>
    std::vector<U> map(const std::vector<T>& source, const F& mapper)
    {
        std::vector<U> mapped{};
        for (VAL& elem : source)
            mapped.emplace_back(mapper(elem));
        return mapped;
    }

    template<typename T, typename F>
    std::vector<T> filter(const std::vector<T>& source, const F& predicate)
    {
        std::vector<T> filtered{};
        for (VAL& elem : source)
            if (predicate(elem))
                filtered.emplace_back(elem);
        return filtered;
    }

    template<typename T, typename S, typename F>
    S fold(const std::vector<T>& source, const S& seed, const F& folder)
    {
        S state(seed);
        for (VAL& elem : source)
            state = folder(state, elem);
        return state;
    }

    template<typename Cr>
    std::vector<typename Cr::value_type> to_vector(const Cr& source)
    {
        CONSTRAIN_AS_CONTAINER(Cr);
        return { source.begin(), source.end() };
    }
}

#endif
