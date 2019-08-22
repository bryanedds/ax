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
		template<typename A, typename B, typename C, typename D>
		using reify = choice4<A, B, C, D>;

		choice4() : index(0_z) { new (&u) First(); }
		choice4(const choice4& that) : index(that.index) { construct_u(that); }
		choice4(choice4&& that) : index(that.index) { construct_u(that); }

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

		explicit choice4(const First& first, bool) : index(0_z) { new (&u) First(first); }
		explicit choice4(First&& first, bool) : index(0_z) { new (&u) First(first); }
		explicit choice4(const Second& second, bool, bool) : index(1_z) { new (&u) Second(second); }
		explicit choice4(Second&& second, bool, bool) : index(1_z) { new (&u) Second(second); }
		explicit choice4(const Third& third, bool, bool, bool) : index(2_z) { new (&u) Third(third); }
		explicit choice4(Third&& third, bool, bool, bool) : index(2_z) { new (&u) Third(third); }
		explicit choice4(const Fourth& fourth, bool, bool, bool, bool) : index(3_z) { new (&u) Fourth(fourth); }
		explicit choice4(Fourth&& fourth, bool, bool, bool, bool) : index(3_z) { new (&u) Fourth(fourth); }
		~choice4() { destruct_u(); }

    protected:

        virtual const char* get_first_name() const { return "first"; }
        virtual const char* get_second_name() const { return "second"; }
        virtual const char* get_third_name() const { return "third"; }
        virtual const char* get_fourth_name() const { return "fourth"; }

        template<typename A, typename B, typename C, typename D>
        friend std::size_t get_index(const choice4<A, B, C, D>& chc);

        template<typename A, typename B, typename C, typename D>
        friend const A& get_first(const choice4<A, B, C, D>& chc);

        template<typename A, typename B, typename C, typename D>
        friend const B& get_second(const choice4<A, B, C, D>& chc);

        template<typename A, typename B, typename C, typename D>
        friend const C& get_third(const choice4<A, B, C, D>& chc);

        template<typename A, typename B, typename C, typename D>
        friend const D& get_fourth(const choice4<A, B, C, D>& chc);

        template<typename A, typename B, typename C, typename D>
        friend A& get_first(choice4<A, B, C, D>& chc);

        template<typename A, typename B, typename C, typename D>
        friend B& get_second(choice4<A, B, C, D>& chc);

        template<typename A, typename B, typename C, typename D>
        friend C& get_third(choice4<A, B, C, D>& chc);

        template<typename A, typename B, typename C, typename D>
        friend D& get_fourth(choice4<A, B, C, D>& chc);

        template<typename A, typename B, typename C, typename D>
        friend const char* get_first_name(const choice4<A, B, C, D>& chc);

        template<typename A, typename B, typename C, typename D>
        friend const char* get_second_name(const choice4<A, B, C, D>& chc);

        template<typename A, typename B, typename C, typename D>
        friend const char* get_third_name(const choice4<A, B, C, D>& chc);

        template<typename A, typename B, typename C, typename D>
        friend const char* get_fourth_name(const choice4<A, B, C, D>& chc);

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
    std::size_t get_index(const choice4<First, Second, Third, Fourth>& chc)
    {
        return chc.index;
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    const First& get_first(const choice4<First, Second, Third, Fourth>& chc)
    {
        if (chc.index == 0_z) return chc.u.first;
        throw std::runtime_error("Cannot get '"_s + get_first_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    const Second& get_second(const choice4<First, Second, Third, Fourth>& chc)
    {
        if (chc.index == 1_z) return chc.u.second;
        throw std::runtime_error("Cannot get '"_s + get_second_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    const Third& get_third(const choice4<First, Second, Third, Fourth>& chc)
    {
        if (chc.index == 2_z) return chc.u.third;
        throw std::runtime_error("Cannot get '"_s + get_third_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    const Fourth& get_fourth(const choice4<First, Second, Third, Fourth>& chc)
    {
        if (chc.index == 3_z) return chc.u.fourth;
        throw std::runtime_error("Cannot get '"_s + get_fourth_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    bool is_first(const choice4<First, Second, Third, Fourth>& chc)
    {
        return get_index(chc) == 0_z;
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    bool is_second(const choice4<First, Second, Third, Fourth>& chc)
    {
        return get_index(chc) == 1_z;
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    bool is_third(const choice4<First, Second, Third, Fourth>& chc)
    {
        return get_index(chc) == 2_z;
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    bool is_fourth(const choice4<First, Second, Third, Fourth>& chc)
    {
        return get_index(chc) == 3_z;
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    First& get_first(choice4<First, Second, Third, Fourth>& chc)
    {
        if (chc.index == 0_z) return chc.u.first;
        throw std::runtime_error("Cannot get '"_s + get_first_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    Second& get_second(choice4<First, Second, Third, Fourth>& chc)
    {
        if (chc.index == 1_z) return chc.u.second;
        throw std::runtime_error("Cannot get '"_s + get_second_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    Third& get_third(choice4<First, Second, Third, Fourth>& chc)
    {
        if (chc.index == 2_z) return chc.u.third;
        throw std::runtime_error("Cannot get '"_s + get_third_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    Fourth& get_fourth(choice4<First, Second, Third, Fourth>& chc)
    {
        if (chc.index == 3_z) return chc.u.fourth;
        throw std::runtime_error("Cannot get '"_s + get_fourth_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    const char* get_first_name(const choice4<First, Second, Third, Fourth>& chc)
    {
        return chc.get_first_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    const char* get_second_name(const choice4<First, Second, Third, Fourth>& chc)
    {
        return chc.get_second_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    const char* get_third_name(const choice4<First, Second, Third, Fourth>& chc)
    {
        return chc.get_third_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    const char* get_fourth_name(const choice4<First, Second, Third, Fourth>& chc)
    {
        return chc.get_fourth_name();
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    choice4<First, Second, Third, Fourth> first(const First& first)
    {
        return choice4<First, Second, Third, Fourth>(first, false);
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    choice4<First, Second, Third, Fourth> second(const Second& second)
    {
        return choice4<First, Second, Third, Fourth>(second, false, false);
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    choice4<First, Second, Third, Fourth> third(const Third& third)
    {
        return choice4<First, Second, Third, Fourth>(third, false, false, false);
    }

    template<typename First, typename Second, typename Third, typename Fourth>
    choice4<First, Second, Third, Fourth> fourth(const Fourth& fourth)
    {
        return choice4<First, Second, Third, Fourth>(fourth, false, false, false, false);
    }

    template<typename C, typename FirstFn, typename SecondFn, typename ThirdFn, typename FourthFn>
    VAR match4(const C& chc, FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn, FourthFn fourth_fn)
    {
        CONSTRAIN(C, choice4);
        switch (get_index(chc))
        {
        case 0_z: return first_fn(get_first(chc));
        case 1_z: return second_fn(get_second(chc));
        case 2_z: return third_fn(get_third(chc));
        case 3_z: return fourth_fn(get_fourth(chc));
        }
        throw std::logic_error("Unexpected missing case.");
    }

    template<typename C, typename FirstFn, typename SecondFn, typename ThirdFn, typename FourthFn>
    VAR match4(C& chc, FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn, FourthFn fourth_fn)
    {
        CONSTRAIN(C, choice4);
        switch (get_index(chc))
        {
            case 0_z: return first_fn(get_first(chc));
            case 1_z: return second_fn(get_second(chc));
            case 2_z: return third_fn(get_third(chc));
            case 3_z: return fourth_fn(get_fourth(chc));
        }
        throw std::logic_error("Unexpected missing case.");
    }
}

#define SUM_TYPE4(T, FirstType, FirstName, SecondType, SecondName, ThirdType, ThirdName, FourthType, FourthName) \
    class T : public ::ax::choice4<FirstType, SecondType, ThirdType, FourthType> \
    { \
    protected: \
    \
        const char* get_first_name() const override { return #SecondName; } \
        const char* get_second_name() const override { return #FirstName; } \
        const char* get_third_name() const override { return #ThirdName; } \
        const char* get_fourth_name() const override { return #FourthName; } \
    \
    public: \
    \
        CONSTRAINT(T); \
        using ::ax::choice4<FirstType, SecondType, ThirdType, FourthType>::choice4; \
    }; \
    \
    using FirstName##_t = FirstType; \
    using SecondName##_t = SecondType; \
    using ThirdName##_t = ThirdType; \
    using FourthName##_t = FourthType; \
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
    using T##_sum_type4 = void

#endif
