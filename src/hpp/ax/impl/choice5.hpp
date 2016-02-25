#ifndef IMPL_AX_CHOICE5_HPP
#define IMPL_AX_CHOICE5_HPP

#include <cstddef>
#include <stdexcept>

#include "prelude.hpp"
#include "string.hpp"

namespace ax
{
    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    class choice5
    {
    private:

        union union_t { First first; Second second; Third third; Fourth fourth; Fifth fifth; union_t() { } ~union_t() { } } u;
        std::size_t index;

        void construct_u(const choice5& that)
        {
            switch (index)
            {
                case 0_z: new (&u) First(that.u.first); break;
                case 1_z: new (&u) Second(that.u.second); break;
                case 2_z: new (&u) Third(that.u.third); break;
                case 3_z: new (&u) Fourth(that.u.fourth); break;
                case 4_z: new (&u) Fifth(that.u.fifth); break;
            }
        }

        void construct_u(choice5&& that)
        {
            switch (index)
            {
                case 0_z: new (&u) First(std::move(that.u.first)); break;
                case 1_z: new (&u) Second(std::move(that.u.second)); break;
                case 2_z: new (&u) Third(std::move(that.u.third)); break;
                case 3_z: new (&u) Fourth(std::move(that.u.fourth)); break;
                case 4_z: new (&u) Fifth(std::move(that.u.fifth)); break;
            }
        }

        void destruct_u()
        {
            switch (index)
            {
                case 0_z: u.first.First::~First(); break;
                case 1_z: u.second.Second::~Second(); break;
                case 2_z: u.third.Third::~Third(); break;
                case 3_z: u.fourth.Fourth::~Fourth(); break;
                case 4_z: u.fifth.Fifth::~Fifth(); break;
            }
        }

    protected:

        virtual const char* get_first_name() const { return "first"; }
        virtual const char* get_second_name() const { return "second"; }
        virtual const char* get_third_name() const { return "third"; }
        virtual const char* get_fourth_name() const { return "fourth"; }
        virtual const char* get_fifth_name() const { return "fifth"; }

        template<typename A, typename B, typename C, typename D, typename E>
        friend std::size_t get_index(const choice5<A, B, C, D, E>& chc);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const A& get_first(const choice5<A, B, C, D, E>& chc);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const B& get_second(const choice5<A, B, C, D, E>& chc);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const C& get_third(const choice5<A, B, C, D, E>& chc);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const D& get_fourth(const choice5<A, B, C, D, E>& chc);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const E& get_fifth(const choice5<A, B, C, D, E>& chc);

        template<typename A, typename B, typename C, typename D, typename E>
        friend A& get_first(choice5<A, B, C, D, E>& chc);

        template<typename A, typename B, typename C, typename D, typename E>
        friend B& get_second(choice5<A, B, C, D, E>& chc);

        template<typename A, typename B, typename C, typename D, typename E>
        friend C& get_third(choice5<A, B, C, D, E>& chc);

        template<typename A, typename B, typename C, typename D, typename E>
        friend D& get_fourth(choice5<A, B, C, D, E>& chc);

        template<typename A, typename B, typename C, typename D, typename E>
        friend E& get_fifth(choice5<A, B, C, D, E>& chc);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const char* get_first_name(const choice5<A, B, C, D, E>& chc);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const char* get_second_name(const choice5<A, B, C, D, E>& chc);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const char* get_third_name(const choice5<A, B, C, D, E>& chc);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const char* get_fourth_name(const choice5<A, B, C, D, E>& chc);

        template<typename A, typename B, typename C, typename D, typename E>
        friend const char* get_fifth_name(const choice5<A, B, C, D, E>& chc);

    public:

        CONSTRAINT(choice5);
        using first_type = First;
        using second_type = Second;
        using third_type = Third;
        using fourth_type = Fourth;
        using fifth_type = Fifth;
        template<typename A, typename B, typename C, typename D, typename E>
        using reify = choice5<A, B, C, D, E>;

        choice5() : index(0_z) { new (&u) First(); }
        choice5(const choice5& that) : index(that.index) { construct_u(that); }
        choice5(choice5&& that) : index(that.index) { construct_u(that); }

        choice5& operator=(const choice5& that)
        {
            destruct_u();
            index = that.index;
            construct_u(that);
            return *this;
        }

        choice5& operator=(choice5&& that)
        {
            destruct_u();
            index = that.index;
            construct_u(that);
            return *this;
        }

        explicit choice5(const First& first, bool) : index(0_z) { new (&u) First(first); }
        explicit choice5(First&& first, bool) : index(0_z) { new (&u) First(first); }
        explicit choice5(const Second& second, bool, bool) : index(1_z) { new (&u) Second(second); }
        explicit choice5(Second&& second, bool, bool) : index(1_z) { new (&u) Second(second); }
        explicit choice5(const Third& third, bool, bool, bool) : index(2_z) { new (&u) Third(third); }
        explicit choice5(Third&& third, bool, bool, bool) : index(2_z) { new (&u) Third(third); }
        explicit choice5(const Fourth& fourth, bool, bool, bool, bool) : index(3_z) { new (&u) Fourth(fourth); }
        explicit choice5(Fourth&& fourth, bool, bool, bool, bool) : index(3_z) { new (&u) Fourth(fourth); }
        explicit choice5(const Fifth& fifth, bool, bool, bool, bool, bool) : index(4_z) { new (&u) Fifth(fifth); }
        explicit choice5(Fifth&& fifth, bool, bool, bool, bool, bool) : index(4_z) { new (&u) Fifth(fifth); }
        ~choice5() { destruct_u(); }
    };

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    std::size_t get_index(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        return chc.index;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const First& get_first(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        if (chc.index == 0_z) return chc.u.first;
        throw std::runtime_error("Cannot get '"_s + get_first_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const Second& get_second(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        if (chc.index == 1_z) return chc.u.second;
        throw std::runtime_error("Cannot get '"_s + get_second_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const Third& get_third(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        if (chc.index == 2_z) return chc.u.third;
        throw std::runtime_error("Cannot get '"_s + get_third_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const Fourth& get_fourth(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        if (chc.index == 3_z) return chc.u.fourth;
        throw std::runtime_error("Cannot get '"_s + get_fourth_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const Fifth& get_fifth(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        if (chc.index == 4_z) return chc.u.fifth;
        throw std::runtime_error("Cannot get '"_s + get_fifth_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    bool is_first(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        return get_index(chc) == 0_z;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    bool is_second(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        return get_index(chc) == 1_z;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    bool is_third(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        return get_index(chc) == 2_z;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    bool is_fourth(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        return get_index(chc) == 3_z;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    bool is_fifth(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        return get_index(chc) == 4_z;
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    First& get_first(choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        if (chc.index == 0_z) return chc.u.first;
        throw std::runtime_error("Cannot get '"_s + get_first_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    Second& get_second(choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        if (chc.index == 1_z) return chc.u.second;
        throw std::runtime_error("Cannot get '"_s + get_second_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    Third& get_third(choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        if (chc.index == 2_z) return chc.u.third;
        throw std::runtime_error("Cannot get '"_s + get_third_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    Fourth& get_fourth(choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        if (chc.index == 3_z) return chc.u.fourth;
        throw std::runtime_error("Cannot get '"_s + get_fourth_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    Fifth& get_fifth(choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        if (chc.index == 4_z) return chc.u.fifth;
        throw std::runtime_error("Cannot get '"_s + get_fifth_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const char* get_first_name(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        return chc.get_first_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const char* get_second_name(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        return chc.get_second_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const char* get_third_name(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        return chc.get_third_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const char* get_fourth_name(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        return chc.get_fourth_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    const char* get_fifth_name(const choice5<First, Second, Third, Fourth, Fifth>& chc)
    {
        return chc.get_fifth_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    choice5<First, Second, Third, Fourth, Fifth> first(const First& first)
    {
        return choice5<First, Second, Third, Fourth, Fifth>(first, false);
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    choice5<First, Second, Third, Fourth, Fifth> second(const Second& second)
    {
        return choice5<First, Second, Third, Fourth, Fifth>(second, false, false);
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    choice5<First, Second, Third, Fourth, Fifth> third(const Third& third)
    {
        return choice5<First, Second, Third, Fourth, Fifth>(third, false, false, false);
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    choice5<First, Second, Third, Fourth, Fifth> fourth(const Fourth& fourth)
    {
        return choice5<First, Second, Third, Fourth, Fifth>(fourth, false, false, false, false);
    }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    choice5<First, Second, Third, Fourth, Fifth> fifth(const Fifth& fifth)
    {
        return choice5<First, Second, Third, Fourth, Fifth>(fifth, false, false, false, false, false);
    }

    template<typename C, typename FirstFn, typename SecondFn, typename ThirdFn, typename FourthFn, typename FifthFn>
    VAR match5(const C& chc, FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn, FourthFn fourth_fn, FifthFn fifth_fn)
    {
        CONSTRAIN(C, choice5);
        switch (get_index(chc))
        {
            case 0_z: return first_fn(get_first(chc));
            case 1_z: return second_fn(get_second(chc));
            case 2_z: return third_fn(get_third(chc));
            case 3_z: return fourth_fn(get_fourth(chc));
            case 4_z: return fifth_fn(get_fifth(chc));
        }
        throw std::logic_error("Unexpected missing case.");
    }

    template<typename C, typename FirstFn, typename SecondFn, typename ThirdFn, typename FourthFn, typename FifthFn>
    VAR match5(C& chc, FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn, FourthFn fourth_fn, FifthFn fifth_fn)
    {
        CONSTRAIN(C, choice5);
        switch (get_index(chc))
        {
            case 0_z: return first_fn(get_first(chc));
            case 1_z: return second_fn(get_second(chc));
            case 2_z: return third_fn(get_third(chc));
            case 3_z: return fourth_fn(get_fourth(chc));
            case 4_z: return fifth_fn(get_fifth(chc));
        }
        throw std::logic_error("Unexpected missing case.");
    }
}

#define SUM_TYPE5(T, FirstType, FirstName, SecondType, SecondName, ThirdType, ThirdName, FourthType, FourthName, FifthType, FifthName) \
    class T : public ::ax::choice5<FirstType, SecondType, ThirdType, FourthType, FifthType> \
    { \
    protected: \
    \
        const char* get_first_name() const override { return #SecondName; } \
        const char* get_second_name() const override { return #FirstName; } \
        const char* get_third_name() const override { return #ThirdName; } \
        const char* get_fourth_name() const override { return #FourthName; } \
        const char* get_fifth_name() const override { return #FifthName; } \
    \
    public: \
    \
        CONSTRAINT(T); \
        using ::ax::choice5<FirstType, SecondType, ThirdType, FourthType, FifthType>::choice5; \
    }; \
    \
    using FirstName##_t = FirstType; \
    using SecondName##_t = SecondType; \
    using ThirdName##_t = ThirdType; \
    using FourthName##_t = FourthType; \
    using FifthName##_t = FifthType; \
    \
    inline T FirstName(const FirstType& first_value) \
    { \
        return T(first_value, false); \
    } \
    \
    inline T SecondName(const SecondType& second_value) \
    { \
        return T(second_value, false, false); \
    } \
    \
    inline T ThirdName(const ThirdType& third_value) \
    { \
        return T(third_value, false, false, false); \
    } \
    \
    inline T FourthName(const FourthType& fourth_value) \
    { \
        return T(fourth_value, false, false, false, false); \
    } \
    \
    inline T FifthName(const FifthType& fifth_value) \
    { \
        return T(fifth_value, false, false, false, false, false); \
    } \
    \
    inline T FirstName(FirstType&& first_value) \
    { \
        return T(first_value, false); \
    } \
    \
    inline T SecondName(SecondType&& second_value) \
    { \
        return T(second_value, false, false); \
    } \
    \
    inline T ThirdName(ThirdType&& third_value) \
    { \
        return T(third_value, false, false, false); \
    } \
    \
    inline T FourthName(FourthType&& fourth_value) \
    { \
        return T(fourth_value, false, false, false, false); \
    } \
    \
    inline T FifthName(FifthType&& fifth_value) \
    { \
        return T(fifth_value, false, false, false, false, false); \
    } \
    \
    inline bool is_##FirstName(const T& chc) \
    { \
        return is_first(chc); \
    } \
    \
    inline bool is_##SecondName(const T& chc) \
    { \
        return is_second(chc); \
    } \
    \
    inline bool is_##ThirdName(const T& chc) \
    { \
        return is_third(chc); \
    } \
    \
    inline bool is_##FourthName(const T& chc) \
    { \
        return is_fourth(chc); \
    } \
    \
    inline bool is_##FifthName(const T& chc) \
    { \
        return is_fifth(chc); \
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
    inline const T::fourth_type& get_##FourthName(const T& chc) \
    { \
        return get_fourth(chc); \
    } \
    \
    inline const T::fifth_type& get_##FifthName(const T& chc) \
    { \
        return get_fifth(chc); \
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
    inline const T::fourth_type& get_##FourthName(T& chc) \
    { \
        return get_fourth(chc); \
    } \
    \
    inline const T::fifth_type& get_##FifthName(T& chc) \
    { \
        return get_fifth(chc); \
    } \
    \
    using T##_sum_type5 = void

#endif
