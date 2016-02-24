#ifndef IMPL_AX_CHOICE_HPP
#define IMPL_AX_CHOICE_HPP

#include <cstddef>
#include <stdexcept>

#include "prelude.hpp"
#include "string.hpp"

namespace ax
{
    // TODO: build choices up to choice7.
    template<typename First, typename Second, typename Third>
    class choice
    {
    private:

        union union_t { First first; Second second; Third third; union_t() { } ~union_t() { } } u;
        std::size_t index;

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

    public:

        CONSTRAINT(choice);
        using first_type = First;
        using second_type = Second;
        using third_type = Third;
        template<typename A, typename B, typename C>
        using reify = choice<A, B, C>;

        choice() : index(0_z)
        {
            new (&u.first) First();
        }

        choice(const choice& that) : index(that.index)
        {
            switch (index)
            {
                case 0_z: new (&u.first) First(that.u.first); break;
                case 1_z: new (&u.second) Second(that.u.second); break;
                case 2_z: new (&u.third) Third(that.u.third); break;
            }
        }

        choice(choice&& that) : index(that.index)
        {
            switch (index)
            {
                case 0_z: new (&u.first) First(std::move(that.u.first)); break;
                case 1_z: new (&u.second) Second(std::move(that.u.second)); break;
                case 2_z: new (&u.third) Third(std::move(that.u.third)); break;
            }
        }

        choice& operator=(const choice& that)
        {
            index = that.index;
            switch (index)
            {
                case 0_z: u.first = that.u.first; break;
                case 1_z: u.second = that.u.second; break;
                case 2_z: u.third = that.u.third; break;
            }
            return *this;
        }

        choice& operator=(choice&& that)
        {
            index = that.index;
            switch (index)
            {
                case 0_z: u.first = std::move(that.u.first); break;
                case 1_z: u.second = std::move(that.u.second); break;
                case 2_z: u.third = std::move(that.u.third); break;
            }
            return *this;
        }

        explicit choice(const First& first) : index(0_z)
        {
            new (&u.first) First(first);
        }

        explicit choice(First&& first) : index(0_z)
        {
            new (&u.first) First(first);
        }

        explicit choice(const Second& second, bool) : index(1_z)
        {
            new (&u.second) Second(second);
        }

        explicit choice(Second&& second, bool) : index(1_z)
        {
            new (&u.second) Second(second);
        }

        explicit choice(const Third& third, bool, bool) : index(2_z)
        {
            new (&u.third) Third(third);
        }

        explicit choice(Third&& third, bool, bool) : index(2_z)
        {
            new (&u.third) Third(third);
        }

        ~choice()
        {
            switch (index)
            {
                case 0_z: u.first.First::~First(); break;
                case 1_z: u.second.Second::~Second(); break;
                case 2_z: u.third.Third::~Third(); break;
            }
        }
    };

    template<typename First, typename Second, typename Third>
    std::size_t get_index(const choice<First, Second, Third>& chc)
    {
        return chc.index;
    }

    template<typename First, typename Second, typename Third>
    const First& get_first(const choice<First, Second, Third>& chc)
    {
        if (chc.index == 0_z) return chc.u.first;
        throw std::runtime_error("Cannot get '"_s + get_first_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third>
    const Second& get_second(const choice<First, Second, Third>& chc)
    {
        if (chc.index == 1_z) return chc.u.second;
        throw std::runtime_error("Cannot get '"_s + get_second_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third>
    const Third& get_third(const choice<First, Second, Third>& chc)
    {
        if (chc.index == 2_z) return chc.u.third;
        throw std::runtime_error("Cannot get '"_s + get_third_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third>
    bool is_first(const choice<First, Second, Third>& chc)
    {
        return get_index(chc) == 0_z;
    }

    template<typename First, typename Second, typename Third>
    bool is_second(const choice<First, Second, Third>& chc)
    {
        return get_index(chc) == 1_z;
    }

    template<typename First, typename Second, typename Third>
    bool is_third(const choice<First, Second, Third>& chc)
    {
        return get_index(chc) == 2_z;
    }

    template<typename First, typename Second, typename Third>
    First& get_first(choice<First, Second, Third>& chc)
    {
        if (chc.index == 0_z) return chc.u.first;
        throw std::runtime_error("Cannot get '"_s + get_first_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third>
    Second& get_second(choice<First, Second, Third>& chc)
    {
        if (chc.index == 1_z) return chc.u.second;
        throw std::runtime_error("Cannot get '"_s + get_second_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third>
    Third& get_third(choice<First, Second, Third>& chc)
    {
        if (chc.index == 2_z) return chc.u.third;
        throw std::runtime_error("Cannot get '"_s + get_third_name(chc) + "' value.");
    }

    template<typename First, typename Second, typename Third>
    const char* get_first_name(const choice<First, Second, Third>& chc)
    {
        return chc.get_first_name();
    }

    template<typename First, typename Second, typename Third>
    const char* get_second_name(const choice<First, Second, Third>& chc)
    {
        return chc.get_second_name();
    }

    template<typename First, typename Second, typename Third>
    const char* get_third_name(const choice<First, Second, Third>& chc)
    {
        return chc.get_third_name();
    }

    template<typename First, typename Second, typename Third>
    choice<First, Second, Third> first(const First& first)
    {
        return choice<First, Second, Third>(first);
    }

    template<typename First, typename Second, typename Third>
    choice<First, Second, Third> second(const Second& second)
    {
        return choice<First, Second, Third>(second, false);
    }

    template<typename First, typename Second, typename Third>
    choice<First, Second, Third> third(const Third& third)
    {
        return choice<First, Second, Third>(third, false, false);
    }

    template<typename C, typename FirstFn, typename SecondFn, typename ThirdFn>
    VAR match3(const C& chc, FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn)
    {
        CONSTRAIN(C, choice);
        switch (get_index(chc))
        {
        case 0_z: return first_fn(get_first(chc));
        case 1_z: return second_fn(get_second(chc));
        default: return third_fn(get_third(chc));
        }
    }

    template<typename C, typename FirstFn, typename SecondFn, typename ThirdFn>
    VAR match3(C& chc, FirstFn first_fn, SecondFn second_fn, ThirdFn third_fn)
    {
        CONSTRAIN(C, choice);
        switch (get_index(chc))
        {
        case 0_z: return first_fn(get_first(chc));
        case 1_z: return second_fn(get_second(chc));
        default: return third_fn(get_third(chc));
        }
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
        return T(first_value); \
    } \
    \
    inline T Sn(const St& second_value) \
    { \
        return T(second_value, false); \
    } \
    \
    inline T Tn(const Tt& third_value) \
    { \
        return T(third_value, false, false); \
    } \
    \
    inline T Fn(Ft&& first_value) \
    { \
        return T(first_value); \
    } \
    \
    inline T Sn(St&& second_value) \
    { \
        return T(second_value, false); \
    } \
    \
    inline T Tn(Tt&& third_value) \
    { \
        return T(third_value, false, false); \
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
