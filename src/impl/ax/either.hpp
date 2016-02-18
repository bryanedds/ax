#ifndef IMPL_AX_EITHER_HPP
#define IMPL_AX_EITHER_HPP

#include <cstddef>
#include <stdexcept>

#include "prelude.hpp"
#include "string.hpp"

namespace ax
{
    template<typename L, typename R>
    class either
    {
    private:

        bool is_right;
        R right;
        L left;

    protected:

        virtual const char* get_right_name() const { return "right"; }
        virtual const char* get_left_name() const { return "left"; }

        template<typename A, typename B>
        friend const B& get_right(const either<A, B>& eir);

        template<typename A, typename B>
        friend const A& get_left(const either<A, B>& eir);

        template<typename A, typename B>
        friend B& get_right(either<A, B>& eir);

        template<typename A, typename B>
        friend A& get_left(either<A, B>& eir);

        template<typename A, typename B>
        friend const char* get_right_name(const either<A, B>& eir);

        template<typename A, typename B>
        friend const char* get_left_name(const either<A, B>& eir);

    public:

        CONSTRAINT(either);
        using right_type = R;
        using left_type = L;
        template<typename A, typename B>
        using reify = either<A, B>;
        
        either() { } // NOTE: do not change this to = default as that makes MSVC think it is deleted when inherited!
        either(const either&) = default;
        either(either&&) = default;
        either& operator=(const either&) = default;
        either& operator=(either&&) = default;
        ~either() = default;

        either(const R& right) : is_right(true), right(right) { }
        either(const L& left, bool) : is_right(false), left(left) { }
        either(R&& right) : is_right(true), right(right) { }
        either(L&& left, bool) : is_right(false), left(left) { }
        explicit operator bool() const { return is_right; }
        explicit operator bool() { return is_right; }

        const R& operator*() const
        {
            if (is_right) return right;
            throw std::runtime_error("Cannot get non-existent right value.");
        }

        R& operator*()
        {
            if (is_right) return right;
            throw std::runtime_error("Cannot get non-existent right value.");
        }

        const L& operator~() const
        {
            if (!is_right) return left;
            throw std::runtime_error("Cannot get non-existent left value.");
        }

        L& operator~()
        {
            if (!is_right) return left;
            throw std::runtime_error("Cannot get non-existent left value.");
        }
    };

    template<typename L, typename R>
    bool is_right(const either<L, R>& eir)
    {
        return static_cast<bool>(eir);
    }

    template<typename L, typename R>
    bool is_left(const either<L, R>& eir)
    {
        return !static_cast<bool>(eir);
    }

    template<typename L, typename R>
    const R& get_right(const either<L, R>& eir)
    {
        if (eir.is_right) return eir.right;
        throw std::runtime_error("Cannot get '"_s + get_right_name(eir) + "' value.");
    }

    template<typename L, typename R>
    const L& get_left(const either<L, R>& eir)
    {
        if (!eir.is_right) return eir.left;
        throw std::runtime_error("Cannot get '"_s + get_left_name(eir) + "' value.");
    }

    template<typename L, typename R>
    R& get_right(either<L, R>& eir)
    {
        if (eir.is_right) return eir.right;
        throw std::runtime_error("Cannot get '"_s + get_right_name(eir) + "' value.");
    }

    template<typename L, typename R>
    L& get_left(either<L, R>& eir)
    {
        if (!eir.is_right) return eir.left;
        throw std::runtime_error("Cannot get '"_s + get_left_name(eir) + "' value.");
    }

    template<typename L, typename R>
    const char* get_right_name(const either<L, R>& eir)
    {
        return eir.get_right_name();
    }

    template<typename L, typename R>
    const char* get_left_name(const either<L, R>& eir)
    {
        return eir.get_left_name();
    }

    template<typename L, typename R>
    either<L, R> right(const R& right)
    {
        return either<L, R>(right);
    }

    template<typename L, typename R>
    either<L, R> left(const L& left)
    {
        return either<L, R>(left, false);
    }

    template<typename L, typename R>
    either<L, R> right(R&& right)
    {
        return either<L, R>(right);
    }

    template<typename L, typename R>
    either<L, R> left(L&& left)
    {
        return either<L, R>(left, false);
    }

    template<typename E, typename Lf, typename Rf>
    auto match2(const E& eir, Rf right_fn, Lf left_fn)
    {
        CONSTRAIN(E, either);
        if (is_right(eir)) return right_fn(get_right(eir));
        return left_fn(get_left(eir));
    }

    template<typename E, typename Lf, typename Rf>
    auto match2(E& eir, Rf right_fn, Lf left_fn)
    {
        CONSTRAIN(E, either);
        if (is_right(eir)) return right_fn(get_right(eir));
        return left_fn(get_left(eir));
    }
}

#define SUM_TYPE(T, Lt, Ln, Rt, Rn) \
    class T : public ::ax::either<Lt, Rt> \
    { \
    protected: \
    \
        const char* get_right_name() const override { return #Ln; } \
        const char* get_left_name() const override { return #Rn; } \
    \
    public: \
    \
        CONSTRAINT(T); \
        using ::ax::either<Lt, Rt>::either; \
    }; \
    \
    inline T Rn(const Rt& right_value) \
    { \
        return T(right_value); \
    } \
    \
    inline T Ln(const Lt& left_value) \
    { \
        return T(left_value, false); \
    } \
    \
    inline T Rn(Rt&& right_value) \
    { \
        return T(right_value); \
    } \
    \
    inline T Ln(Lt&& left_value) \
    { \
        return T(left_value, false); \
    } \
    \
    inline bool is_##Rn(const T& eir) \
    { \
        return is_right(eir); \
    } \
    \
    inline bool is_##Ln(const T& eir) \
    { \
        return is_left(eir); \
    } \
    \
    inline const T::right_type& get_##Rn(const T& eir) \
    { \
        return get_right(eir); \
    } \
    \
    inline const T::left_type& get_##Ln(const T& eir) \
    { \
        return get_left(eir); \
    } \
    \
    inline T::right_type& get_##Rn(T& eir) \
    { \
        return get_right(eir); \
    } \
    \
    inline T::left_type& get_##Ln(T& eir) \
    { \
        return get_left(eir); \
    } \
    \
    using T##_sum_type = void

#endif
