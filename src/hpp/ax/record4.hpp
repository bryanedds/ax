#ifndef AX_RECORD4_HPP
#define AX_RECORD4_HPP

#include <cstddef>
#include <stdexcept>

#include "prelude.hpp"

namespace ax
{
    template<typename First, typename Second, typename Third, typename Fourth>
    class record4
    {
    public:

        CONSTRAINT(record4);
        using first_type = First;
        using second_type = Second;
        using third_type = Third;
        using fourth_type = Fourth;
        template<typename A, typename B, typename C, typename D> using reify = ax::record4<A, B, C, D>;

        record4() { } // NOTE: do not change this to = default as that makes MSVC think it is deleted when inherited!
        record4(const record4&) = default;
        record4(record4&&) = default;

        record4(const First& first, const Second& second, const Third& third, const Fourth& fourth) :
            first(first), second(second), third(third), fourth(fourth) { }

        record4(First&& first, Second&& second, Third&& third, Fourth&& fourth) :
            first(first), second(second), third(third), fourth(fourth) { }

        virtual ~record4() = default;

        record4& operator=(const record4&) = default;
        record4& operator=(record4&&) = default;

        const First& fst() const { return first; }
        const Second& snd() const { return second; }
        const Third& thd() const { return third; }
        const Fourth& frth() const { return fourth; }

        First& fst() { return first; }
        Second& snd() { return second; }
        Third& thd() { return third; }
        Fourth& frth() { return fourth; }

        virtual const char* get_first_name() const { return "first"; }
        virtual const char* get_second_name() const { return "second"; }
        virtual const char* get_third_name() const { return "third"; }
        virtual const char* get_fourth_name() const { return "fourth"; }

    private:

        First first;
        Second second;
        Third third;
        Fourth fourth;
    };

    template<typename First, typename Second, typename Third, typename Fourth>
    const First& fst(const record4<First, Second, Third, Fourth>& rcd) { return rcd.fst(); }

    template<typename First, typename Second, typename Third, typename Fourth>
    const Second& snd(const record4<First, Second, Third, Fourth>& rcd) { return rcd.snd(); }

    template<typename First, typename Second, typename Third, typename Fourth>
    const Third& thd(const record4<First, Second, Third, Fourth>& rcd) { return rcd.thd(); }

    template<typename First, typename Second, typename Third, typename Fourth>
    const Fourth& frth(const record4<First, Second, Third, Fourth>& rcd) { return rcd.frth(); }

    template<typename First, typename Second, typename Third, typename Fourth>
    First& fst(ax::record4<First, Second, Third, Fourth>& rcd) { return rcd.fst(); }

    template<typename First, typename Second, typename Third, typename Fourth>
    Second& snd(ax::record4<First, Second, Third, Fourth>& rcd) { return rcd.snd(); }

    template<typename First, typename Second, typename Third, typename Fourth>
    Third& thd(ax::record4<First, Second, Third, Fourth>& rcd) { return rcd.thd(); }

    template<typename First, typename Second, typename Third, typename Fourth>
    Fourth& frth(ax::record4<First, Second, Third, Fourth>& rcd) { return rcd.frth(); }

    template<typename First, typename Second, typename Third, typename Fourth>
    ax::record4<First, Second, Third, Fourth> make_record4(const First& first, const Second& second, const Third& third, const Fourth& fourth)
    {
        return ax::record4<First, Second, Third, Fourth>(first, second, third, fourth);
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    ax::record4<First, Second, Third, Fourth> make_record4(First&& first, Second&& second, Third&& third, Fourth&& fourth)
    {
        return ax::record4<First, Second, Third, Fourth>(first, second, third, fourth);
    }
}

#define PRODUCT_TYPE4(T, FirstType, FirstName, SecondType, SecondName, ThirdType, ThirdName, FourthType, FourthName) \
    class T : public ::ax::record4<FirstType, SecondType, ThirdType, FourthType> \
    { \
    public: \
    \
        CONSTRAINT(T); \
        using ::ax::record4<FirstType, SecondType, ThirdType, FourthType>::record4; \
        \
        inline const T::first_type& get_##FirstName(const T& rcd) { return fst(rcd); } \
        inline const T::second_type& get_##SecondName(const T& rcd) { return snd(rcd); } \
        inline const T::third_type& get_##ThirdName(const T& rcd) { return thd(rcd); } \
        inline const T::fourth_type& get_##FourthName(const T& rcd) { return frth(rcd); } \
        \
        inline T::first_type& get_##FirstName(T& rcd) { return fst(rcd); } \
        inline T::second_type& get_##SecondName(T& rcd) { return snd(rcd); } \
        inline T::third_type& get_##ThirdName(T& rcd) { return thd(rcd); } \
        inline T::fourth_type& get_##FourthName(T& rcd) { return frth(rcd); } \
        \
        const char* get_first_name() const override { return #FirstName; } \
        const char* get_second_name() const override { return #SecondName; } \
        const char* get_third_name() const override { return #ThirdName; } \
        const char* get_fourth_name() const override { return #FourthName; } \
    }; \
    \
    T make_##T(const T::first_type& first, const T::second_type& second, const T::third_type& third, const T::fourth_type& fourth) \
    { \
        return T(first, second, third, fourth); \
    } \
    \
    T make_##T(T::first_type&& first, T::second_type&& second, T::third_type&& third, T::fourth_type&& fourth) \
    { \
        return T(first, second, third, fourth); \
    } \
    \
    using T##_product_type4 = void

#endif
