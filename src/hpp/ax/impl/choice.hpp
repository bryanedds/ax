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

        template<typename FirstFn, typename SecondFn, typename ThirdFn>
        VAR match(FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn) const
        {
            switch (get_index())
            {
                case 0_z: return first_fn(get_first());
                case 1_z: return second_fn(get_second());
                case 2_z: return third_fn(get_third());
            }
            throw std::logic_error("Unexpected missing case.");
        }

        template<typename FirstFn, typename SecondFn, typename ThirdFn>
        VAR match(FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn)
        {
            switch (get_index())
            {
                case 0_z: return first_fn(get_first());
                case 1_z: return second_fn(get_second());
                case 2_z: return third_fn(get_third());
            }
            throw std::logic_error("Unexpected missing case.");
        }

        std::size_t get_index() const { return index; }

        const F& get_first() const
        {
            if (index == 0_z) return u.first;
            throw std::runtime_error("Cannot get '"_s + get_first_name() + "' value.");
        }

        const S& get_second() const
        {
            if (index == 1_z) return u.second;
            throw std::runtime_error("Cannot get '"_s + get_second_name() + "' value.");
        }

        const T& get_third() const
        {
            if (index == 2_z) return u.third;
            throw std::runtime_error("Cannot get '"_s + get_third_name() + "' value.");
        }

        bool is_first() const { return get_index() == 0_z; }
        bool is_second() const { return get_index() == 1_z; }
        bool is_third() const { return get_index() == 2_z; }

        F& get_first()
        {
            if (index == 0_z) return u.first;
            throw std::runtime_error("Cannot get '"_s + get_first_name() + "' value.");
        }

        S& get_second()
        {
            if (index == 1_z) return u.second;
            throw std::runtime_error("Cannot get '"_s + get_second_name() + "' value.");
        }

        T& get_third()
        {
            if (index == 2_z) return u.third;
            throw std::runtime_error("Cannot get '"_s + get_third_name() + "' value.");
        }

        virtual const char* get_first_name() const { return "first"; }
        virtual const char* get_second_name() const { return "second"; }
        virtual const char* get_third_name() const { return "third"; }

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
    choice<F, S, T> first(const F& first) { return choice<F, S, T>(first, false); }

    template<typename F, typename S, typename T>
    choice<F, S, T> second(const S& second) { return choice<F, S, T>(second, false, false); }

    template<typename F, typename S, typename T>
    choice<F, S, T> third(const T& third) { return choice<F, S, T>(third, false, false, false); }
}

#define SUM_TYPE3(T, Ft, Fn, St, Sn, Tt, Tn) \
    class T : public ::ax::choice<Ft, St, Tt> \
    { \
    public: \
    \
        CONSTRAINT(T); \
        using ::ax::choice<Ft, St, Tt>::choice; \
        \
        inline bool is_##Fn() const { return is_first(); } \
        inline bool is_##Sn() const { return is_second(); } \
        inline bool is_##Tn() const { return is_third(); } \
        \
        inline const T::first_type& get_##Fn() const { return get_first(); } \
        inline const T::second_type& get_##Sn() const { return get_second(); } \
        inline const T::third_type& get_##Tn() const { return get_third(); } \
        \
        inline T::first_type& get_##Fn() { return get_first(); } \
        inline T::second_type& get_##Sn() { return get_second(); } \
        inline T::third_type& get_##Tn() { return get_third(); } \
        \
        const char* get_first_name() const override { return #Sn; } \
        const char* get_second_name() const override { return #Fn; } \
        const char* get_third_name() const override { return #Tn; } \
    }; \
    \
    using Fn##_t = Ft; \
    using Sn##_t = St; \
    using Tn##_t = Tt; \
    \
    inline T Fn(const Ft& first_value) { return T(first_value, false); } \
    inline T Sn(const St& second_value) { return T(second_value, false, false); } \
    inline T Tn(const Tt& third_value) { return T(third_value, false, false, false); } \
    \
    inline T Fn(Ft&& first_value) { return T(first_value, false); } \
    inline T Sn(St&& second_value) { return T(second_value, false, false); } \
    inline T Tn(Tt&& third_value) { return T(third_value, false, false, false); } \
    \
    using T##_sum_type3 = void

#endif
