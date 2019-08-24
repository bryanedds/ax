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
        virtual ~choice5() { destruct_u(); }

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

        template<typename FirstFn, typename SecondFn, typename ThirdFn, typename FourthFn, typename FifthFn>
        VAR match(FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn, FourthFn fourth_fn, FifthFn fifth_fn) const
        {
            switch (get_index())
            {
                case 0_z: return first_fn(get_first());
                case 1_z: return second_fn(get_second());
                case 2_z: return third_fn(get_third());
                case 3_z: return fourth_fn(get_fourth());
                case 4_z: return fifth_fn(get_fifth());
            }
            throw std::logic_error("Unexpected missing case.");
        }

        template<typename FirstFn, typename SecondFn, typename ThirdFn, typename FourthFn, typename FifthFn>
        VAR match(FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn, FourthFn fourth_fn, FifthFn fifth_fn)
        {
            switch (get_index())
            {
                case 0_z: return first_fn(get_first());
                case 1_z: return second_fn(get_second());
                case 2_z: return third_fn(get_third());
                case 3_z: return fourth_fn(get_fourth());
                case 4_z: return fifth_fn(get_fifth());
            }
            throw std::logic_error("Unexpected missing case.");
        }

        std::size_t get_index() const { return index; }

        const First& get_first() const
        {
            if (index == 0_z) return u.first;
            throw std::runtime_error("Cannot get '"_s + get_first_name() + "' value.");
        }

        const Second& get_second() const
        {
            if (index == 1_z) return u.second;
            throw std::runtime_error("Cannot get '"_s + get_second_name() + "' value.");
        }

        const Third& get_third() const
        {
            if (index == 2_z) return u.third;
            throw std::runtime_error("Cannot get '"_s + get_third_name() + "' value.");
        }

        const Fourth& get_fourth() const
        {
            if (index == 3_z) return u.fourth;
            throw std::runtime_error("Cannot get '"_s + get_fourth_name() + "' value.");
        }

        const Fifth& get_fifth() const
        {
            if (index == 4_z) return u.fifth;
            throw std::runtime_error("Cannot get '"_s + get_fifth_name() + "' value.");
        }

        bool is_first() const { return get_index() == 0_z; }
        bool is_second() const { return get_index() == 1_z; }
        bool is_third() const { return get_index() == 2_z; }
        bool is_fourth() const { return get_index() == 3_z; }
        bool is_fifth() const { return get_index() == 4_z; }

        First& get_first()
        {
            if (index == 0_z) return u.first;
            throw std::runtime_error("Cannot get '"_s + get_first_name() + "' value.");
        }

        Second& get_second()
        {
            if (index == 1_z) return u.second;
            throw std::runtime_error("Cannot get '"_s + get_second_name() + "' value.");
        }

        Third& get_third()
        {
            if (index == 2_z) return u.third;
            throw std::runtime_error("Cannot get '"_s + get_third_name() + "' value.");
        }

        Fourth& get_fourth()
        {
            if (index == 3_z) return u.fourth;
            throw std::runtime_error("Cannot get '"_s + get_fourth_name() + "' value.");
        }

        Fifth& get_fifth()
        {
            if (index == 4_z) return u.fifth;
            throw std::runtime_error("Cannot get '"_s + get_fifth_name() + "' value.");
        }

        virtual const char* get_first_name() const { return "first"; }
        virtual const char* get_second_name() const { return "second"; }
        virtual const char* get_third_name() const { return "third"; }
        virtual const char* get_fourth_name() const { return "fourth"; }
        virtual const char* get_fifth_name() const { return "fifth"; }

    private:

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

        union union_t { First first; Second second; Third third; Fourth fourth; Fifth fifth; union_t() { } ~union_t() { } } u;
        std::size_t index;
    };

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    choice5<First, Second, Third, Fourth, Fifth> first(const First& first) { return choice5<First, Second, Third, Fourth, Fifth>(first, false); }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    choice5<First, Second, Third, Fourth, Fifth> second(const Second& second) { return choice5<First, Second, Third, Fourth, Fifth>(second, false, false); }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    choice5<First, Second, Third, Fourth, Fifth> third(const Third& third) { return choice5<First, Second, Third, Fourth, Fifth>(third, false, false, false); }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    choice5<First, Second, Third, Fourth, Fifth> fourth(const Fourth& fourth) { return choice5<First, Second, Third, Fourth, Fifth>(fourth, false, false, false, false); }

    template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
    choice5<First, Second, Third, Fourth, Fifth> fifth(const Fifth& fifth) { return choice5<First, Second, Third, Fourth, Fifth>(fifth, false, false, false, false, false); }
}

#define SUM_TYPE5(T, FirstType, FirstName, SecondType, SecondName, ThirdType, ThirdName, FourthType, FourthName, FifthType, FifthName) \
    class T : public ::ax::choice5<FirstType, SecondType, ThirdType, FourthType, FifthType> \
    { \
    public: \
    \
        CONSTRAINT(T); \
        using ::ax::choice5<FirstType, SecondType, ThirdType, FourthType, FifthType>::choice5; \
        \
        inline bool is_##FirstName() const { return is_first(); } \
        inline bool is_##SecondName() const { return is_second(); } \
        inline bool is_##ThirdName() const { return is_third(); } \
        inline bool is_##FourthName() const { return is_fourth(); } \
        inline bool is_##FifthName() const { return is_fifth(); } \
        \
        inline const T::first_type& get_##FirstName() const { return get_first(); } \
        inline const T::second_type& get_##SecondName() const { return get_second(); } \
        inline const T::third_type& get_##ThirdName() const { return get_third(); } \
        inline const T::fourth_type& get_##FourthName() const { return get_fourth(); } \
        inline const T::fifth_type& get_##FifthName() const { return get_fifth(); } \
        \
        inline T::first_type& get_##FirstName() { return get_first(); } \
        inline T::second_type& get_##SecondName() { return get_second(); } \
        inline T::third_type& get_##ThirdName() { return get_third(); } \
        inline T::fourth_type& get_##FourthName() { return get_fourth(); } \
        inline const T::fifth_type& get_##FifthName() { return get_fifth(); } \
        \
        const char* get_first_name() const override { return #SecondName; } \
        const char* get_second_name() const override { return #FirstName; } \
        const char* get_third_name() const override { return #ThirdName; } \
        const char* get_fourth_name() const override { return #FourthName; } \
        const char* get_fifth_name() const override { return #FifthName; } \
    }; \
    \
    using FirstName##_t = FirstType; \
    using SecondName##_t = SecondType; \
    using ThirdName##_t = ThirdType; \
    using FourthName##_t = FourthType; \
    using FifthName##_t = FifthType; \
    \
    inline T FirstName(const FirstType& first_value) { return T(first_value, false); } \
    inline T SecondName(const SecondType& second_value) { return T(second_value, false, false); } \
    inline T ThirdName(const ThirdType& third_value) { return T(third_value, false, false, false); } \
    inline T FourthName(const FourthType& fourth_value) { return T(fourth_value, false, false, false, false); } \
    inline T FifthName(const FifthType& fifth_value) { return T(fifth_value, false, false, false, false, false); } \
    \
    inline T FirstName(FirstType&& first_value) { return T(first_value, false); } \
    inline T SecondName(SecondType&& second_value) { return T(second_value, false, false); } \
    inline T ThirdName(ThirdType&& third_value) { return T(third_value, false, false, false); } \
    inline T FourthName(FourthType&& fourth_value) { return T(fourth_value, false, false, false, false); } \
    inline T FifthName(FifthType&& fifth_value) { return T(fifth_value, false, false, false, false, false); } \
    \
    using T##_sum_type5 = void

#endif
