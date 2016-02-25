#ifndef IMPL_AX_RECORD5_HPP
#define IMPL_AX_RECORD5_HPP

#include <cstddef>
#include <stdexcept>

#include "prelude.hpp"

namespace ax
{
    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    class record5
    {
    private:

        First first;
        Second second;
        Third third;
        Fourth fourth;
        Fifth fifth;

    protected:

        virtual const char* get_first_name() const { return "first"; }
        virtual const char* get_second_name() const { return "second"; }
        virtual const char* get_third_name() const { return "third"; }
        virtual const char* get_fourth_name() const { return "fourth"; }
        virtual const char* get_fifth_name() const { return "fifth"; }

        template<typename A, typename B, typename C, typename D, typename E>
        friend const A& fst(const record5<A, B, C, D, E>& rcd);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const B& snd(const record5<A, B, C, D, E>& rcd);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const C& thd(const record5<A, B, C, D, E>& rcd);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const D& fourth(const record5<A, B, C, D, E>& rcd);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const E& fifth(const record5<A, B, C, D, E>& rcd);

        template<typename A, typename B, typename C, typename D, typename E>
        friend A& fst(record5<A, B, C, D, E>& rcd);

        template<typename A, typename B, typename C, typename D, typename E>
        friend B& snd(record5<A, B, C, D, E>& rcd);

        template<typename A, typename B, typename C, typename D, typename E>
        friend C& thd(record5<A, B, C, D, E>& rcd);

        template<typename A, typename B, typename C, typename D, typename E>
        friend D& fourth(record5<A, B, C, D, E>& rcd);

        template<typename A, typename B, typename C, typename D, typename E>
        friend E& fifth(record5<A, B, C, D, E>& rcd);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const char* get_first_name(const record5<A, B, C, D, E>& rcd);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const char* get_second_name(const record5<A, B, C, D, E>& rcd);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const char* get_third_name(const record5<A, B, C, D, E>& rcd);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const char* get_fourth_name(const record5<A, B, C, D, E>& rcd);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const char* get_fifth_name(const record5<A, B, C, D, E>& rcd);

    public:

        CONSTRAINT(record5);
        using first_type = First;
        using second_type = Second;
        using third_type = Third;
        using fourth_type = Fourth;
        using fifth_type = Fifth;
        template<typename A, typename B, typename C, typename D, typename E>
        using reify = record5<A, B, C, D, E>;

        record5() { } // NOTE: do not change this to = default as that makes MSVC think it is deleted when inherited!
        record5(const record5&) = default;
        record5(record5&&) = default;
        record5& operator=(const record5&) = default;
        record5& operator=(record5&&) = default;

        record5(const First& first, const Second& second, const Third& third, const Fourth& fourth, const Fifth& fifth) :
            first(first), second(second), third(third), fourth(fourth), fifth(fifth) { }

        record5(First&& first, Second&& second, Third&& third, Fourth&& fourth, Fifth&& fifth) :
            first(first), second(second), third(third), fourth(fourth), fifth(fifth) { }
    };

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const First& fst(const record5<First, Second, Third, Fourth, Fifth>& rcd)
    {
        return rcd.first;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const Second& snd(const record5<First, Second, Third, Fourth, Fifth>& rcd)
    {
        return rcd.second;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const Third& thd(const record5<First, Second, Third, Fourth, Fifth>& rcd)
    {
        return rcd.third;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const Fourth& fourth(const record5<First, Second, Third, Fourth, Fifth>& rcd)
    {
        return rcd.fourth;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const Fifth& fifth(const record5<First, Second, Third, Fourth, Fifth>& rcd)
    {
        return rcd.fifth;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    First& fst(record5<First, Second, Third, Fourth, Fifth>& rcd)
    {
        return rcd.first;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    Second& snd(record5<First, Second, Third, Fourth, Fifth>& rcd)
    {
        return rcd.second;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    Third& thd(record5<First, Second, Third, Fourth, Fifth>& rcd)
    {
        return rcd.third;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    Fourth& fourth(record5<First, Second, Third, Fourth, Fifth>& rcd)
    {
        return rcd.fourth;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    Fifth& fifth(record5<First, Second, Third, Fourth, Fifth>& rcd)
    {
        return rcd.fifth;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const char* get_first_name(const record5<First, Second, Third, Fourth, Fifth>& rcd)
    {
        return rcd.get_first_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const char* get_second_name(const record5<First, Second, Third, Fourth, Fifth>& rcd)
    {
        return rcd.get_second_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const char* get_third_name(const record5<First, Second, Third, Fourth, Fifth>& rcd)
    {
        return rcd.get_third_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const char* get_fourth_name(const record5<First, Second, Third, Fourth, Fifth>& rcd)
    {
        return rcd.get_fourth_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const char* get_fifth_name(const record5<First, Second, Third, Fourth, Fifth>& rcd)
    {
        return rcd.get_fifth_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    record5<First, Second, Third, Fourth, Fifth> make_record5(
        const First& first, const Second& second, const Third& third, const Fourth& fourth, const Fifth& fifth)
    {
        return record5<First, Second, Third, Fourth, Fifth>(first, second, third, fourth, fifth);
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    record5<First, Second, Third, Fourth, Fifth> make_record5(
        First&& first, Second&& second, Third&& third, Fourth&& fourth, Fifth&& fifth)
    {
        return record5<First, Second, Third, Fourth, Fifth>(first, second, third, fourth, fifth);
    }
}

#define PRODUCT_TYPE5(T, FirstType, FirstName, SecondType, SecondName, ThirdType, ThirdName, FourthType, FourthName, FifthType, FifthName) \
    class T : public ::ax::record5<FirstType, SecondType, ThirdType, FourthType, FifthType> \
    { \
    protected: \
    \
        const char* get_first_name() const override { return #FirstName; } \
        const char* get_second_name() const override { return #SecondName; } \
        const char* get_third_name() const override { return #ThirdName; } \
        const char* get_fourth_name() const override { return #FourthName; } \
        const char* get_fifth_name() const override { return #FifthName; } \
    \
    public: \
    \
        CONSTRAINT(T); \
        using ::ax::record5<FirstType, SecondType, ThirdType, FourthType, FifthType>::record5; \
    }; \
    \
    inline const T::first_type& get_##FirstName(const T& rcd) \
    { \
        return fst(rcd); \
    } \
    \
    inline const T::second_type& get_##SecondName(const T& rcd) \
    { \
        return snd(rcd); \
    } \
    \
    inline const T::third_type& get_##ThirdName(const T& rcd) \
    { \
        return thd(rcd); \
    } \
    \
    inline const T::fourth_type& get_##FourthName(const T& rcd) \
    { \
        return fourth(rcd); \
    } \
    \
    inline const T::fifth_type& get_##FifthName(const T& rcd) \
    { \
        return fifth(rcd); \
    } \
    \
    inline T::first_type& get_##FirstName(T& rcd) \
    { \
        return fst(rcd); \
    } \
    \
    inline T::second_type& get_##SecondName(T& rcd) \
    { \
        return snd(rcd); \
    } \
    \
    inline T::third_type& get_##ThirdName(T& rcd) \
    { \
        return thd(rcd); \
    } \
    \
    inline T::fourth_type& get_##FourthName(T& rcd) \
    { \
        return fourth(rcd); \
    } \
    \
    inline T::fifth_type& get_##FifthName(T& rcd) \
    { \
        return fifth(rcd); \
    } \
    \
    T make_##T(const T::first_type& first, const T::second_type& second, const T::third_type& third, const T::fourth_type& fourth, const T::fifth_type& fifth) \
    { \
        return T(first, second, third, fourth, fifth); \
    } \
    \
    T make_##T(T::first_type&& first, T::second_type&& second, T::third_type&& third, T::fourth_type&& fourth, T::fifth_type&& fifth) \
    { \
        return T(first, second, third, fourth, fifth); \
    } \
    \
    using T##_product_type5 = void

#endif
