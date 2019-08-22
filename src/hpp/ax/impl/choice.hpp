#ifndef IMPL_AX_CHOICE_HPP
#define IMPL_AX_CHOICE_HPP

#include <cstddef>
#include <stdexcept>

#include "prelude.hpp"
#include "string.hpp"

namespace ax
{
    template<typename F, typename S, typename T>
    class choice
    {
	public:

		CONSTRAINT(choice);
		using first_type = F;
		using second_type = S;
		using third_type = T;
		template<typename A, typename B, typename C>
		using reify = choice<A, B, C>;

		choice() : index(0_z) { new (&u) F(); }
		choice(const choice& that) : index(that.index) { construct_u(that); }
		choice(choice&& that) : index(that.index) { construct_u(that); }

		choice& operator=(const choice& that)
		{
			destruct_u();
			index = that.index;
			construct_u(that);
			return *this;
		}

		choice& operator=(choice&& that)
		{
			destruct_u();
			index = that.index;
			construct_u(that);
			return *this;
		}

		explicit choice(const F& first, bool) : index(0_z) { new (&u) F(first); }
		explicit choice(F&& first, bool) : index(0_z) { new (&u) F(first); }
		explicit choice(const S& second, bool, bool) : index(1_z) { new (&u) S(second); }
		explicit choice(S&& second, bool, bool) : index(1_z) { new (&u) S(second); }
		explicit choice(const T& third, bool, bool, bool) : index(2_z) { new (&u) T(third); }
		explicit choice(T&& third, bool, bool, bool) : index(2_z) { new (&u) T(third); }
		~choice() { destruct_u(); }

    protected:

        virtual const char* get_first_name() const { return "first"; }
        virtual const char* get_second_name() const { return "second"; }
        virtual const char* get_third_name() const { return "third"; }

        template<typename A, typename B, typename C>
        friend std::size_t get_index(const choice<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend const A& get_first(const choice<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend const B& get_second(const choice<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend const C& get_third(const choice<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend A& get_first(choice<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend B& get_second(choice<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend C& get_third(choice<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend const char* get_first_name(const choice<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend const char* get_second_name(const choice<A, B, C>& chc);

        template<typename A, typename B, typename C>
        friend const char* get_third_name(const choice<A, B, C>& chc);
	private:

		void construct_u(const choice& that)
		{
			switch (index)
			{
				case 0_z: new (&u) F(that.u.first); break;
				case 1_z: new (&u) S(that.u.second); break;
				case 2_z: new (&u) T(that.u.third); break;
			}
		}

		void construct_u(choice&& that)
		{
			switch (index)
			{
				case 0_z: new (&u) F(std::move(that.u.first)); break;
				case 1_z: new (&u) S(std::move(that.u.second)); break;
				case 2_z: new (&u) T(std::move(that.u.third)); break;
			}
		}

		void destruct_u()
		{
			switch (index)
			{
				case 0_z: u.first.F::~F(); break;
				case 1_z: u.second.S::~S(); break;
				case 2_z: u.third.T::~T(); break;
			}
		}

		union union_t { F first; S second; T third; union_t() { } ~union_t() { } } u;
		std::size_t index;
    };

    template<typename F, typename S, typename T>
    std::size_t get_index(const choice<F, S, T>& chc)
    {
        return chc.index;
    }

    template<typename F, typename S, typename T>
    const F& get_first(const choice<F, S, T>& chc)
    {
        if (chc.index == 0_z) return chc.u.first;
        throw std::runtime_error("Cannot get '"_s + get_first_name(chc) + "' value.");
    }

    template<typename F, typename S, typename T>
    const S& get_second(const choice<F, S, T>& chc)
    {
        if (chc.index == 1_z) return chc.u.second;
        throw std::runtime_error("Cannot get '"_s + get_second_name(chc) + "' value.");
    }

    template<typename F, typename S, typename T>
    const T& get_third(const choice<F, S, T>& chc)
    {
        if (chc.index == 2_z) return chc.u.third;
        throw std::runtime_error("Cannot get '"_s + get_third_name(chc) + "' value.");
    }

    template<typename F, typename S, typename T>
    bool is_first(const choice<F, S, T>& chc)
    {
        return get_index(chc) == 0_z;
    }

    template<typename F, typename S, typename T>
    bool is_second(const choice<F, S, T>& chc)
    {
        return get_index(chc) == 1_z;
    }

    template<typename F, typename S, typename T>
    bool is_third(const choice<F, S, T>& chc)
    {
        return get_index(chc) == 2_z;
    }

    template<typename F, typename S, typename T>
    F& get_first(choice<F, S, T>& chc)
    {
        if (chc.index == 0_z) return chc.u.first;
        throw std::runtime_error("Cannot get '"_s + get_first_name(chc) + "' value.");
    }

    template<typename F, typename S, typename T>
    S& get_second(choice<F, S, T>& chc)
    {
        if (chc.index == 1_z) return chc.u.second;
        throw std::runtime_error("Cannot get '"_s + get_second_name(chc) + "' value.");
    }

    template<typename F, typename S, typename T>
    T& get_third(choice<F, S, T>& chc)
    {
        if (chc.index == 2_z) return chc.u.third;
        throw std::runtime_error("Cannot get '"_s + get_third_name(chc) + "' value.");
    }

    template<typename F, typename S, typename T>
    const char* get_first_name(const choice<F, S, T>& chc)
    {
        return chc.get_first_name();
    }

    template<typename F, typename S, typename T>
    const char* get_second_name(const choice<F, S, T>& chc)
    {
        return chc.get_second_name();
    }

    template<typename F, typename S, typename T>
    const char* get_third_name(const choice<F, S, T>& chc)
    {
        return chc.get_third_name();
    }

    template<typename F, typename S, typename T>
    choice<F, S, T> first(const F& first)
    {
        return choice<F, S, T>(first, false);
    }

    template<typename F, typename S, typename T>
    choice<F, S, T> second(const S& second)
    {
        return choice<F, S, T>(second, false, false);
    }

    template<typename F, typename S, typename T>
    choice<F, S, T> third(const T& third)
    {
        return choice<F, S, T>(third, false, false, false);
    }

    template<typename C, typename FirstFn, typename SecondFn, typename ThirdFn>
    VAR match3(const C& chc, FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn)
    {
        CONSTRAIN(C, choice);
        switch (get_index(chc))
        {
        case 0_z: return first_fn(get_first(chc));
        case 1_z: return second_fn(get_second(chc));
        case 2_z: return third_fn(get_third(chc));
        }
        throw std::logic_error("Unexpected missing case.");
    }

    template<typename C, typename FirstFn, typename SecondFn, typename ThirdFn>
    VAR match3(C& chc, FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn)
    {
        CONSTRAIN(C, choice);
        switch (get_index(chc))
        {
        case 0_z: return first_fn(get_first(chc));
        case 1_z: return second_fn(get_second(chc));
        case 2_z: return third_fn(get_third(chc));
        }
        throw std::logic_error("Unexpected missing case.");
    }
}

#define SUM_TYPE3(T, Ft, Fn, St, Sn, Tt, Tn) \
    class T : public ::ax::choice<Ft, St, Tt> \
    { \
    protected: \
    \
        const char* get_first_name() const override { return #Sn; } \
        const char* get_second_name() const override { return #Fn; } \
        const char* get_third_name() const override { return #Tn; } \
    \
    public: \
    \
        CONSTRAINT(T); \
        using ::ax::choice<Ft, St, Tt>::choice; \
    }; \
    \
    using Fn##_t = Ft; \
    using Sn##_t = St; \
    using Tn##_t = Tt; \
    \
    inline T Fn(const Ft& first_value) \
    { \
        return T(first_value, false); \
    } \
    \
    inline T Sn(const St& second_value) \
    { \
        return T(second_value, false, false); \
    } \
    \
    inline T Tn(const Tt& third_value) \
    { \
        return T(third_value, false, false, false); \
    } \
    \
    inline T Fn(Ft&& first_value) \
    { \
        return T(first_value, false); \
    } \
    \
    inline T Sn(St&& second_value) \
    { \
        return T(second_value, false, false); \
    } \
    \
    inline T Tn(Tt&& third_value) \
    { \
        return T(third_value, false, false, false); \
    } \
    \
    inline bool is_##Fn(const T& chc) \
    { \
        return is_first(chc); \
    } \
    \
    inline bool is_##Sn(const T& chc) \
    { \
        return is_second(chc); \
    } \
    \
    inline bool is_##Tn(const T& chc) \
    { \
        return is_third(chc); \
    } \
    \
    inline const T::first_type& get_##Fn(const T& chc) \
    { \
        return get_first(chc); \
    } \
    \
    inline const T::second_type& get_##Sn(const T& chc) \
    { \
        return get_second(chc); \
    } \
    \
    inline const T::third_type& get_##Tn(const T& chc) \
    { \
        return get_third(chc); \
    } \
    \
    inline T::first_type& get_##Fn(T& chc) \
    { \
        return get_first(chc); \
    } \
    \
    inline T::second_type& get_##Sn(T& chc) \
    { \
        return get_second(chc); \
    } \
    \
    inline T::third_type& get_##Tn(T& chc) \
    { \
        return get_third(chc); \
    } \
    \
    using T##_sum_type3 = void

#endif
