#ifndef IMPL_AX_PAIR_HPP
#define IMPL_AX_PAIR_HPP

#include <cstddef>
#include <stdexcept>

#include "prelude.hpp"

namespace ax
{
    template<typename First, typename Second>
    class pair
    {
    public:

        CONSTRAINT(pair);
        using first_type = First;
        using second_type = Second;
        template<typename A, typename B>
        using reify = ax::pair<A, B>;

        pair() { } // NOTE: do not change this to = default as that makes MSVC think it is deleted when inherited!
        pair(const pair&) = default;
        pair(pair&&) = default;
        pair(const First& first, const Second& second) : first(first), second(second) { }
        pair(First&& first, Second&& second) : first(first), second(second) { }
        virtual ~pair() = default;

        pair& operator=(const pair&) = default;
        pair& operator=(pair&&) = default;

        const First& fst() const { return first; }
        const Second& snd() const { return second; }
        First& fst() { return first; }
        Second& snd() { return second; }

        virtual const char* get_first_name() const { return "first"; }
        virtual const char* get_second_name() const { return "second"; }

    private:

        First first;
        Second second;
    };

    template<typename First, typename Second>
    const First& fst(const ax::pair<First, Second>& pr) { return pr.fst(); }

    template<typename First, typename Second>
    const Second& snd(const ax::pair<First, Second>& pr) { return pr.snd(); }

    template<typename First, typename Second>
    First& fst(ax::pair<First, Second>& pr) { return pr.fst(); }

    template<typename First, typename Second>
    Second& snd(ax::pair<First, Second>& pr) { return pr.snd(); }

    template<typename First, typename Second>
    ax::pair<First, Second> make_pair(const First& first, const Second& second)
    {
        return ax::pair<First, Second>(first, second);
    }

    template<typename First, typename Second>
    ax::pair<First, Second> make_pair(First&& first, Second&& second)
    {
        return ax::pair<First, Second>(first, second);
    }
}

#define PRODUCT_TYPE(T, Ft, Fn, St, Sn) \
    class T : public ::ax::pair<Ft, St> \
    { \
    public: \
    \
        CONSTRAINT(T); \
        using ::ax::pair<Ft, St>::pair; \
        \
        inline const T::first_type& get_##Fn() const { return fst(); } \
        inline const T::second_type& get_##Sn() const { return snd(); } \
        \
        inline T::first_type& get_##Fn() { return fst(); } \
        inline T::second_type& get_##Sn() { return snd(); } \
    \
    protected: \
    \
        const char* get_first_name() const override { return #Sn; } \
        const char* get_second_name() const override { return #Fn; } \
    }; \
    \
    T make_##T(const T::first_type& first, const T::second_type& second) \
    { \
        return T(first, second); \
    } \
    \
    T make_##T(T::first_type&& first, T::second_type&& second) \
    { \
        return T(first, second); \
    } \
    \
    using T##_product_type = void

#endif
