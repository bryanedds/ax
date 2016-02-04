#ifndef ax_choice3_hpp
#define ax_choice3_hpp

#include <cstddef>
#include <stdexcept>
#include <functional>

#include "prelude.hpp"

namespace ax
{
    // TODO: build choices up to choice7.
    template<typename First, typename Second, typename Third>
    class choice3
    {
    private:

        std::size_t index;
        First first;
        Second second;
        Third third;

    protected:

        virtual const char* get_first_name() const { return "first"; }
        virtual const char* get_second_name() const { return "second"; }
        virtual const char* get_third_name() const { return "third"; }

        template<typename A, typename B, typename C>
        friend std::size_t get_index(const choice3<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend const A& get_first(const choice3<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend const B& get_second(const choice3<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend const C& get_third(const choice3<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend A& get_first(choice3<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend B& get_second(choice3<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend C& get_third(choice3<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend const char* get_first_name(const choice3<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend const char* get_second_name(const choice3<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend const char* get_third_name(const choice3<A, B, C>& chc);

    public:

        constraint(choice3);

        using first_type = First;
        using second_type = Second;
        using third_type = Third;
        
        template<typename A, typename B, typename C>
        using reify = choice3<A, B, C>;

        choice3() { } // NOTE: do not change this to = default as that makes MSVC think it is deleted when inherited!
        choice3(const choice3&) = default;
        choice3(choice3&&) = default;
        choice3& operator=(const choice3&) = default;
        choice3& operator=(choice3&&) = default;
        ~choice3() = default;

        explicit choice3(const First& first) : index(0_z), first(first) { }
        explicit choice3(const Second& second, bool) : index(1_z), second(second) { }
        explicit choice3(const Third& third, bool, bool) : index(2_z), third(third) { }
        explicit choice3(First&& first) : index(0_z), first(first) { }
        explicit choice3(Second&& second, bool) : index(1_z), second(second) { }
        explicit choice3(Third&& third, bool, bool) : index(2_z), third(third) { }
        
        explicit operator bool() const { return index == 0_z; }
        explicit operator bool() { return index == 0_z; }
    };

    template<typename First, typename Second, typename Third>
    std::size_t get_index(const choice3<First, Second, Third>& chc)
    {
        return chc.index;
    }

    template<typename First, typename Second, typename Third>
    const First& get_first(const choice3<First, Second, Third>& chc)
    {
        if (chc.index == 0_z) return chc.first;
        throw std::runtime_error("Cannot get '"_s + get_first_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third>
    const Second& get_second(const choice3<First, Second, Third>& chc)
    {
        if (chc.index == 1_z) return chc.second;
        throw std::runtime_error("Cannot get '"_s + get_second_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third>
    const Third& get_third(const choice3<First, Second, Third>& chc)
    {
        if (chc.index == 2_z) return chc.third;
        throw std::runtime_error("Cannot get '"_s + get_third_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third>
    First& get_first(choice3<First, Second, Third>& chc)
    {
        if (chc.index == 0_z) return chc.first;
        throw std::runtime_error("Cannot get '"_s + get_first_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third>
    Second& get_second(choice3<First, Second, Third>& chc)
    {
        if (chc.index == 1_z) return chc.second;
        throw std::runtime_error("Cannot get '"_s + get_second_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third>
    Third& get_third(choice3<First, Second, Third>& chc)
    {
        if (chc.index == 2_z) return chc.third;
        throw std::runtime_error("Cannot get '"_s + get_third_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third>
    const char* get_first_name(const choice3<First, Second, Third>& chc)
    {
        return chc.get_first_name();
    }

    template<typename First, typename Second, typename Third>
    const char* get_second_name(const choice3<First, Second, Third>& chc)
    {
        return chc.get_second_name();
    }

    template<typename First, typename Second, typename Third>
    const char* get_third_name(const choice3<First, Second, Third>& chc)
    {
        return chc.get_third_name();
    }

    template<typename First, typename Second, typename Third>
    choice3<First, Second, Third> first(const First& first)
    {
        return choice3<First, Second, Third>(first);
    }

    template<typename First, typename Second, typename Third>
    choice3<First, Second, Third> second(const Second& second)
    {
        return choice3<First, Second, Third>(second, false);
    }

    template<typename First, typename Second, typename Third>
    choice3<First, Second, Third> third(const Third& third)
    {
        return choice3<First, Second, Third>(third, false, false);
    }

    template<typename C, typename FirstFn, typename SecondFn, typename ThirdFn>
    var match3(const C& chc, FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn)
    {
        constrain(C, choice3);
        switch (get_index(chc))
        {
        case 0_z: return first_fn(get_first(chc));
        case 1_z: return second_fn(get_second(chc));
        default: return third_fn(get_third(chc));
        }
    }

    template<typename C, typename FirstFn, typename SecondFn, typename ThirdFn>
    var match3(C& chc, FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn)
    {
        constrain(C, choice3);
        switch (get_index(chc))
        {
        case 0_z: return first_fn(get_first(chc));
        case 1_z: return second_fn(get_second(chc));
        default: return third_fn(get_third(chc));
        }
    }
}

#define sum_type3(T, FirstType, FirstName, SecondType, SecondName, ThirdType, ThirdName) \
    class T : public choice3<FirstType, SecondType, ThirdType> \
    { \
    protected: \
    \
        const char* get_first_name() const override { return #SecondName; } \
        const char* get_second_name() const override { return #FirstName; } \
        const char* get_third_name() const override { return #ThirdName; } \
    \
    public: \
    \
        constraint(T); \
        using choice3<FirstType, SecondType, ThirdType>::choice3; \
    }; \
    \
    inline T FirstName(const FirstType& first_value) \
    { \
        return T(first_value); \
    } \
    \
    inline T SecondName(const SecondType& second_value) \
    { \
        return T(second_value, false); \
    } \
    \
    inline T ThirdName(const ThirdType& third_value) \
    { \
        return T(third_value, false, false); \
    } \
    \
    inline T FirstName(FirstType&& first_mvb) \
    { \
        return T(first_mvb); \
    } \
    \
    inline T SecondName(SecondType&& second_mvb) \
    { \
        return T(second_mvb, false); \
    } \
    \
    inline T ThirdName(ThirdType&& third_mvb) \
    { \
        return T(third_mvb, false, false); \
    } \
    \
    inline const T::first_type& get_##FirstName(const T& chc) \
    { \
        return get_first(chc); \
    } \
    \
    inline const T::second_type& get_##SecondName(const T& chc) \
    { \
        return get_second(chc); \
    } \
    \
    inline const T::third_type& get_##ThirdName(const T& chc) \
    { \
        return get_third(chc); \
    } \
    \
    inline T::first_type& get_##FirstName(T& chc) \
    { \
        return get_first(chc); \
    } \
    \
    inline T::second_type& get_##SecondName(T& chc) \
    { \
        return get_second(chc); \
    } \
    \
    inline T::third_type& get_##ThirdName(T& chc) \
    { \
        return get_third(chc); \
    } \
    \
    using T##_sum_type3 = void

#endif
