#ifndef AX_PAIR_HPP
#define AX_PAIR_HPP

#include <cstddef>
#include <stdexcept>

#include "prelude.hpp"

namespace ax
{
    template<typename First, typename Second>
    class pair
    {
    private:

        First first;
        Second second;

    protected:

        virtual const char* get_first_name() const { return "first"; }
        virtual const char* get_second_name() const { return "second"; }

        template<typename A, typename B>
        friend const A& fst(const pair<A, B>& pr);

        template<typename A, typename B>
        friend const B& snd(const pair<A, B>& pr);

        template<typename A, typename B>
        friend A& fst(pair<A, B>& pr);

        template<typename A, typename B>
        friend B& snd(pair<A, B>& pr);

        template<typename A, typename B>
        friend const char* get_first_name(const pair<A, B>& pr);

        template<typename A, typename B>
        friend const char* get_second_name(const pair<A, B>& pr);

    public:

        constraint(pair);
        using first_type = First;
        using second_type = Second;
        template<typename A, typename B>
        using reify = pair<A, B>;
        
        pair() { } // NOTE: do not change this to = default as that makes MSVC think it is deleted when inherited!
        pair(const pair&) = default;
        pair(pair&&) = default;
        pair& operator=(const pair&) = default;
        pair& operator=(pair&&) = default;
        ~pair() = default;

        pair(const First& first, const Second& second) : first(first), second(second) { }
        pair(First&& first_mvb, Second&& second_mvb) : first(first_mvb), second(second_mvb) { }
    };

    template<typename First, typename Second>
    const First& fst(const pair<First, Second>& pr)
    {
        return pr.first;
    }

    template<typename First, typename Second>
    const Second& snd(const pair<First, Second>& pr)
    {
        return pr.second;
    }

    template<typename First, typename Second>
    First& fst(pair<First, Second>& pr)
    {
        return pr.first;
    }

    template<typename First, typename Second>
    Second& snd(pair<First, Second>& pr)
    {
        return pr.second;
    }

    template<typename First, typename Second>
    const char* get_first_name(const pair<First, Second>& pr)
    {
        return pr.get_first_name();
    }

    template<typename First, typename Second>
    const char* get_second_name(const pair<First, Second>& pr)
    {
        return pr.get_second_name();
    }

    template<typename First, typename Second>
    pair<First, Second> make_pair(const First& first, const Second& second)
    {
        return pair<First, Second>(first, second);
    }

    template<typename First, typename Second>
    pair<First, Second> make_pair(First&& first, Second&& second)
    {
        return pair<First, Second>(first, second);
    }
}

#define product_type(T, Ft, Fn, St, Sn) \
    class T : public pair<Ft, St> \
    { \
    protected: \
    \
        const char* get_first_name() const override { return #Sn; } \
        const char* get_second_name() const override { return #Fn; } \
    \
    public: \
    \
        constraint(T); \
        using pair<Ft, St>::pair; \
    }; \
    \
    inline const T::first_type& Fn(const T& pr) \
    { \
        return fst(pr); \
    } \
    \
    inline const T::second_type& Sn(const T& pr) \
    { \
        return snd(pr); \
    } \
    \
    inline T::first_type& Fn(T& pr) \
    { \
        return fst(pr); \
    } \
    \
    inline T::second_type& Sn(T& pr) \
    { \
        return snd(pr); \
    } \
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
