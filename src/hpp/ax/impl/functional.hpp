#ifndef IMPL_AX_FUNCTIONAL_HPP
#define IMPL_AX_FUNCTIONAL_HPP

#include <cstddef>
#include <vector>

#include "prelude.hpp"
#include "vector.hpp"

namespace ax
{
    // NOTE: here we provide basic functional operators for our common container types, std::vector and ax::vector.

    template<typename U, typename T, typename F>
    ax::vector<U> map(const ax::vector<T>& source, const F& mapper)
    {
        ax::vector<U> mapped;
        for (VAL& elem : source) mapped.push_back(mapper(elem));
        return mapped;
    }

    template<typename U, typename T, typename F>
    std::vector<U> map(const std::vector<T>& source, const F& mapper)
    {
        std::vector<U> mapped;
        for (VAL& elem : source) mapped.push_back(mapper(elem));
        return mapped;
    }

    template<typename T, typename F>
    ax::vector<T> filter(const ax::vector<T>& source, const F& predicate)
    {
        ax::vector<T> filtered;
        for (VAL& elem : source)
            if (predicate(elem))
                filtered.push_back(elem);
        return filtered;
    }

    template<typename T, typename F>
    std::vector<T> filter(const std::vector<T>& source, const F& predicate)
    {
        std::vector<T> filtered;
        for (VAL& elem : source)
            if (predicate(elem))
                filtered.push_back(elem);
        return filtered;
    }

    template<typename T, typename S, typename F>
    S fold(const ax::vector<T>& source, const S& seed, const F& folder)
    {
        S state(seed);
        for (VAL& elem : source) state = folder(state, elem);
        return state;
    }

    template<typename T, typename S, typename F>
    S fold(const std::vector<T>& source, const S& seed, const F& folder)
    {
        S state(seed);
        for (VAL& elem : source) state = folder(state, elem);
        return state;
    }

    template<typename Cr>
    ax::vector<typename Cr::value_type> to_ax_vector(const Cr& source)
    {
        CONSTRAIN_AS_CONTAINER(Cr);
        return { source.begin(), source.end() };
    }

    template<typename Cr>
    std::vector<typename Cr::value_type> to_std_vector(const Cr& source)
    {
        CONSTRAIN_AS_CONTAINER(Cr);
        return { source.begin(), source.end() };
    }
}

#endif
