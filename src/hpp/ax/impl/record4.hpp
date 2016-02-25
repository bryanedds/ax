#ifndef IMPL_AX_RECORD4_HPP
#define IMPL_AX_RECORD4_HPP

#include <cstddef>
#include <stdexcept>

#include "prelude.hpp"

namespace ax
{
    template<typename First, typename Second, typename Third, typename Fourth>
    class record4
    {
    private:

        First first;
        Second second;
        Third third;
        Fourth fourth;

    protected:

        virtual const char* get_first_name() const { return "first"; }
        virtual const char* get_second_name() const { return "second"; }
        virtual const char* get_third_name() const { return "third"; }
        virtual const char* get_fourth_name() const { return "fourth"; }

        template<typename A, typename B, typename C, typename D>
        friend const A& fst(const record4<A, B, C, D>& rcd);

        template<typename A, typename B, typename C, typename D>
        friend const B& snd(const record4<A, B, C, D>& rcd);

        template<typename A, typename B, typename C, typename D>
        friend const C& thd(const record4<A, B, C, D>& rcd);

        template<typename A, typename B, typename C, typename D>
        friend const D& fourth(const record4<A, B, C, D>& rcd);

        template<typename A, typename B, typename C, typename D>
        friend A& fst(record4<A, B, C, D>& rcd);

        template<typename A, typename B, typename C, typename D>
        friend B& snd(record4<A, B, C, D>& rcd);

        template<typename A, typename B, typename C, typename D>
        friend C& thd(record4<A, B, C, D>& rcd);

        template<typename A, typename B, typename C, typename D>
        friend D& fourth(record4<A, B, C, D>& rcd);

        template<typename A, typename B, typename C, typename D>
        friend const char* get_first_name(const record4<A, B, C, D>& rcd);

        template<typename A, typename B, typename C, typename D>
        friend const char* get_second_name(const record4<A, B, C, D>& rcd);

        template<typename A, typename B, typename C, typename D>
        friend const char* get_third_name(const record4<A, B, C, D>& rcd);

        template<typename A, typename B, typename C, typename D>
        friend const char* get_fourth_name(const record4<A, B, C, D>& rcd);

    public:

        CONSTRAINT(record4);
        using first_type = First;
        using second_type = Second;
        using third_type = Third;
        using fourth_type = Fourth;
        template<typename A, typename B, typename C, typename D>
        using reify = record4<A, B, C, D>;
        
        record4() { } // NOTE: do not change this to = default as that makes MSVC think it is deleted when inherited!
        record4(const record4&) = default;
        record4(record4&&) = default;
        record4& operator=(const record4&) = default;
        record4& operator=(record4&&) = default;

        record4(const First& first, const Second& second, const Third& third, const Fourth& fourth) :
            first(first), second(second), third(third), fourth(fourth) { }
        
        record4(First&& first, Second&& second, Third&& third, Fourth&& fourth) :
            first(first), second(second), third(third), fourth(fourth) { }
    };

    template<typename First, typename Second, typename Third, typename Fourth>
    const First& fst(const record4<First, Second, Third, Fourth>& rcd)
    {
        return rcd.first;
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    const Second& snd(const record4<First, Second, Third, Fourth>& rcd)
    {
        return rcd.second;
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    const Third& thd(const record4<First, Second, Third, Fourth>& rcd)
    {
        return rcd.third;
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    const Fourth& fourth(const record4<First, Second, Third, Fourth>& rcd)
    {
        return rcd.fourth;
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    First& fst(record4<First, Second, Third, Fourth>& rcd)
    {
        return rcd.first;
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    Second& snd(record4<First, Second, Third, Fourth>& rcd)
    {
        return rcd.second;
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    Third& thd(record4<First, Second, Third, Fourth>& rcd)
    {
        return rcd.third;
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    Fourth& fourth(record4<First, Second, Third, Fourth>& rcd)
    {
        return rcd.fourth;
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    const char* get_first_name(const record4<First, Second, Third, Fourth>& rcd)
    {
        return rcd.get_first_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    const char* get_second_name(const record4<First, Second, Third, Fourth>& rcd)
    {
        return rcd.get_second_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    const char* get_third_name(const record4<First, Second, Third, Fourth>& rcd)
    {
        return rcd.get_third_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    const char* get_fourth_name(const record4<First, Second, Third, Fourth>& rcd)
    {
        return rcd.get_fourth_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    record4<First, Second, Third, Fourth> make_record4(const First& first, const Second& second, const Third& third, const Fourth& fourth)
    {
        return record4<First, Second, Third, Fourth>(first, second, third, fourth);
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    record4<First, Second, Third, Fourth> make_record4(First&& first, Second&& second, Third&& third, Fourth&& fourth)
    {
        return record4<First, Second, Third, Fourth>(first, second, third, fourth);
    }
}

#define PRODUCT_TYPE3(T, FirstType, FirstName, SecondType, SecondName, ThirdType, ThirdName, FourthType, FourthName) \
    class T : public ::ax::record4<FirstType, SecondType, ThirdType, FourthType> \
    { \
    protected: \
    \
        const char* get_first_name() const override { return #FirstName; } \
        const char* get_second_name() const override { return #SecondName; } \
        const char* get_third_name() const override { return #ThirdName; } \
        const char* get_fourth_name() const override { return #FourthName; } \
    \
    public: \
    \
        CONSTRAINT(T); \
        using ::ax::record4<FirstType, SecondType, ThirdType, FourthType>::record4; \
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
