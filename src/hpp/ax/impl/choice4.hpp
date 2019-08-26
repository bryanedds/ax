#ifndef IMPL_AX_CHOICE4_HPP
#define IMPL_AX_CHOICE4_HPP

#include <cstddef>
#include <stdexcept>

#include "prelude.hpp"
#include "string.hpp"

namespace ax
{
    template<typename First, typename Second, typename Third, typename Fourth>
    class choice4
    {
    public:

        CONSTRAINT(choice4);
        using first_type = First;
        using second_type = Second;
        using third_type = Third;
        using fourth_type = Fourth;
        template<typename A, typename B, typename C, typename D> using reify = ax::choice4<A, B, C, D>;

        choice4() : index(0_z) { new (&u) First(); }
        choice4(const choice4& that) : index(that.index) { construct_u(that); }
        choice4(choice4&& that) : index(that.index) { construct_u(that); }
        explicit choice4(const First& first, bool) : index(0_z) { new (&u) First(first); }
        explicit choice4(First&& first, bool) : index(0_z) { new (&u) First(first); }
        explicit choice4(const Second& second, bool, bool) : index(1_z) { new (&u) Second(second); }
        explicit choice4(Second&& second, bool, bool) : index(1_z) { new (&u) Second(second); }
        explicit choice4(const Third& third, bool, bool, bool) : index(2_z) { new (&u) Third(third); }
        explicit choice4(Third&& third, bool, bool, bool) : index(2_z) { new (&u) Third(third); }
        explicit choice4(const Fourth& fourth, bool, bool, bool, bool) : index(3_z) { new (&u) Fourth(fourth); }
        explicit choice4(Fourth&& fourth, bool, bool, bool, bool) : index(3_z) { new (&u) Fourth(fourth); }
        virtual ~choice4() = default;

        choice4& operator=(const choice4& that)
        {
            destruct_u();
            index = that.index;
            construct_u(that);
            return *this;
        }

        choice4& operator=(choice4&& that)
        {
            destruct_u();
            index = that.index;
            construct_u(that);
            return *this;
        }

        template<typename FirstFn, typename SecondFn, typename ThirdFn, typename FourthFn>
        VAR match(FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn, FourthFn fourth_fn) const
        {
            switch (get_index())
            {
                case 0_z: return first_fn(get_first());
                case 1_z: return second_fn(get_second());
                case 2_z: return third_fn(get_third());
                case 3_z: return fourth_fn(get_fourth());
            }
            throw std::logic_error("Unexpected missing case.");
        }

        template<typename FirstFn, typename SecondFn, typename ThirdFn, typename FourthFn>
        VAR match(FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn, FourthFn fourth_fn)
        {
            switch (get_index())
            {
                case 0_z: return first_fn(get_first());
                case 1_z: return second_fn(get_second());
                case 2_z: return third_fn(get_third());
                case 3_z: return fourth_fn(get_fourth());
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

        bool is_first() const { return get_index() == 0_z; }
        bool is_second() const { return get_index() == 1_z; }
        bool is_third() const { return get_index() == 2_z; }
        bool is_fourth() const { return get_index() == 3_z; }

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

        virtual const char* get_first_name() const { return "first"; }
        virtual const char* get_second_name() const { return "second"; }
        virtual const char* get_third_name() const { return "third"; }
        virtual const char* get_fourth_name() const { return "fourth"; }

    private:

        void construct_u(const choice4& that)
        {
            switch (index)
            {
                case 0_z: new (&u) First(that.u.first); break;
                case 1_z: new (&u) Second(that.u.second); break;
                case 2_z: new (&u) Third(that.u.third); break;
                case 3_z: new (&u) Fourth(that.u.fourth); break;
            }
        }

        void construct_u(choice4&& that)
        {
            switch (index)
            {
                case 0_z: new (&u) First(std::move(that.u.first)); break;
                case 1_z: new (&u) Second(std::move(that.u.second)); break;
                case 2_z: new (&u) Third(std::move(that.u.third)); break;
                case 3_z: new (&u) Fourth(std::move(that.u.fourth)); break;
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
            }
        }

        union union_t { First first; Second second; Third third; Fourth fourth; union_t() { } ~union_t() { } } u;
        std::size_t index;
    };

    template<typename First, typename Second, typename Third, typename Fourth>
    ax::choice4<First, Second, Third, Fourth> first(const First& first) { return ax::choice4<First, Second, Third, Fourth>(first, false); }

    template<typename First, typename Second, typename Third, typename Fourth>
    ax::choice4<First, Second, Third, Fourth> second(const Second& second) { return ax::choice4<First, Second, Third, Fourth>(second, false, false); }

    template<typename First, typename Second, typename Third, typename Fourth>
    ax::choice4<First, Second, Third, Fourth> third(const Third& third) { return ax::choice4<First, Second, Third, Fourth>(third, false, false, false); }

    template<typename First, typename Second, typename Third, typename Fourth>
    ax::choice4<First, Second, Third, Fourth> fourth(const Fourth& fourth) { return ax::choice4<First, Second, Third, Fourth>(fourth, false, false, false, false); }
}

#define SUM_TYPE4(T, FirstType, FirstName, SecondType, SecondName, ThirdType, ThirdName, FourthType, FourthName) \
    class T : public ::ax::choice4<FirstType, SecondType, ThirdType, FourthType> \
    { \
    public: \
    \
        CONSTRAINT(T); \
        using ::ax::choice4<FirstType, SecondType, ThirdType, FourthType>::choice4; \
        \
        inline bool is_##FirstName() const { return is_first(); } \
        inline bool is_##SecondName() const { return is_second(); } \
        inline bool is_##ThirdName() const { return is_third(); } \
        inline bool is_##FourthName() const { return is_fourth(); } \
        \
        inline const T::first_type& get_##FirstName() const { return get_first(); } \
        inline const T::second_type& get_##SecondName() const { return get_second(); } \
        inline const T::third_type& get_##ThirdName() const { return get_third(); } \
        inline const T::fourth_type& get_##FourthName() const { return get_fourth(); } \
        \
        inline T::first_type& get_##FirstName() { return get_first(); } \
        inline T::second_type& get_##SecondName() { return get_second(); } \
        inline T::third_type& get_##ThirdName() { return get_third(); } \
        inline T::fourth_type& get_##FourthName() { return get_fourth(); } \
        \
        const char* get_first_name() const override { return #SecondName; } \
        const char* get_second_name() const override { return #FirstName; } \
        const char* get_third_name() const override { return #ThirdName; } \
        const char* get_fourth_name() const override { return #FourthName; } \
    }; \
    \
    using FirstName##_t = FirstType; \
    using SecondName##_t = SecondType; \
    using ThirdName##_t = ThirdType; \
    using FourthName##_t = FourthType; \
    \
    inline T FirstName(const FirstType& first_value) { return T(first_value, false); } \
    inline T SecondName(const SecondType& second_value) { return T(second_value, false, false); } \
    inline T ThirdName(const ThirdType& third_value) { return T(third_value, false, false, false); } \
    inline T FourthName(const FourthType& fourth_value) { return T(fourth_value, false, false, false, false); } \
    \
    inline T FirstName(FirstType&& first_value) { return T(first_value, false); } \
    inline T SecondName(SecondType&& second_value) { return T(second_value, false, false); } \
    inline T ThirdName(ThirdType&& third_value) { return T(third_value, false, false, false); } \
    inline T FourthName(FourthType&& fourth_value) { return T(fourth_value, false, false, false, false); } \
    \
    using T##_sum_type4 = void

#endif
