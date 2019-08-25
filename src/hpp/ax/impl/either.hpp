#ifndef IMPL_AX_EITHER_HPP
#define IMPL_AX_EITHER_HPP

#include <cstddef>
#include <stdexcept>

#include "prelude.hpp"
#include "string.hpp"

namespace ax
{
    template<typename R, typename L>
    class either
    {
    public:

        CONSTRAINT(either);
        using right_type = R;
        using left_type = L;
        template<typename A, typename B>
        using reify = either<A, B>;

        either() : is_right_flag(true) { new (&u) R(); }

        either(const ax::either& that) : is_right_flag(that.is_right_flag)
        {
            if (is_right_flag) new (&u) R(that.u.right);
            else new (&u) L(that.u.left);
        }

        either(ax::either&& that) : is_right_flag(that.is_right_flag)
        {
            if (is_right_flag) new (&u) R(std::move(that.u.right));
            else new (&u) L(std::move(that.u.left));
        }

        explicit either(const R& right, bool) : is_right_flag(true) { new (&u) R(right); }
        explicit either(R&& right, bool) : is_right_flag(true) { new (&u) R(right); }
        explicit either(const L& left, bool, bool) : is_right_flag(false) { new (&u) L(left); }
        explicit either(L&& left, bool, bool) : is_right_flag(false) { new (&u) L(left); }

        virtual ~either()
        {
            if (is_right_flag) u.right.R::~R();
            else u.left.L::~L();
        }

        ax::either& operator=(const ax::either& that)
        {
            if (is_right_flag) u.right.R::~R();
            else u.left.L::~L();
            is_right_flag = that.is_right_flag;
            if (is_right_flag) new (&u) R(that.u.right);
            else new (&u) L(that.u.left);
            return *this;
        }

        ax::either& operator=(ax::either&& that)
        {
            if (is_right_flag) u.right.R::~R();
            else u.left.L::~L();
            is_right_flag = that.is_right_flag;
            if (is_right_flag) new (&u) R(std::move(that.u.right));
            else new (&u) L(std::move(that.u.left));
            return *this;
        }

        const R& operator*() const
        {
            if (is_right_flag) return u.right;
            throw std::runtime_error("Cannot get non-existent right value.");
        }

        R& operator*()
        {
            if (is_right_flag) return u.right;
            throw std::runtime_error("Cannot get non-existent right value.");
        }

        const L& operator~() const
        {
            if (!is_right_flag) return u.left;
            throw std::runtime_error("Cannot get non-existent left value.");
        }

        L& operator~()
        {
            if (!is_right_flag) return u.left;
            throw std::runtime_error("Cannot get non-existent left value.");
        }

        explicit operator bool() const { return is_right_flag; }
        explicit operator bool() { return is_right_flag; }

        template<typename Rf, typename Lf>
        auto match(Rf right_fn, Lf left_fn) const
        {
            if (is_right_flag) return right_fn(get_right());
            return left_fn(get_left());
        }

        template<typename Rf, typename Lf>
        auto match(Rf right_fn, Lf left_fn)
        {
            if (is_right_flag) return right_fn(get_right());
            return left_fn(get_left());
        }

        bool is_right() const { return static_cast<bool>(*this); }
        bool is_left() const { return !static_cast<bool>(*this); }

        const R& get_right() const
        {
            if (is_right_flag) return u.right;
            throw std::runtime_error("Cannot get '"_s + get_right_name() + "' value.");
        }

        const L& get_left() const
        {
            if (!is_right_flag) return u.left;
            throw std::runtime_error("Cannot get '"_s + get_left_name() + "' value.");
        }

        R& get_right()
        {
            if (is_right_flag) return u.right;
            throw std::runtime_error("Cannot get '"_s + get_right_name() + "' value.");
        }

        L& get_left()
        {
            if (!is_right_flag) return u.left;
            throw std::runtime_error("Cannot get '"_s + get_left_name() + "' value.");
        }

        virtual const char* get_right_name() const { return "right"; }
        virtual const char* get_left_name() const { return "left"; }

    private:

        union union_t { R right; L left; union_t() { } ~union_t() { } } u;
        bool is_right_flag;
    };

    template<typename R, typename L>
    ax::either<R, L> right(R&& right) { return ax::either<R, L>(right, false); }

    template<typename R, typename L>
    ax::either<R, L> left(L&& left) { return ax::either<R, L>(left, false, false); }
}

#define SUM_TYPE(T, Rt, Rn, Lt, Ln) \
    class T : public ::ax::either<Rt, Lt> \
    { \
    public: \
    \
        CONSTRAINT(T); \
        using ::ax::either<Rt, Lt>::either; \
        \
        inline bool is_##Rn() { return is_right(); } \
        inline bool is_##Ln() { return is_left(); } \
        \
        inline const T::right_type& get_##Rn() { return get_right(); } \
        inline const T::left_type& get_##Ln() { return get_left(); } \
        \
        inline T::right_type& get_##Rn() { return get_right(); } \
        inline T::left_type& get_##Ln() { return get_left(); } \
        \
        const char* get_right_name() const override { return #Rn; } \
        const char* get_left_name() const override { return #Ln; } \
    }; \
    \
    using Rt##_t = Rn; \
    using Lt##_t = Ln; \
    \
    inline T Rn(const Rt& right_value) { return T(right_value, false); } \
    inline T Ln(const Lt& left_value) { return T(left_value, false, false); } \
    \
    inline T Rn(Rt&& right_value) { return T(right_value, false); } \
    inline T Ln(Lt&& left_value) { return T(left_value, false, false); } \
    \
    using T##_sum_type = void

#endif
