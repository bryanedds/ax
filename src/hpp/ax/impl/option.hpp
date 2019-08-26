#ifndef IMPL_AX_OPTION_HPP
#define IMPL_AX_OPTION_HPP

#include <cstddef>
#include <stdexcept>

#include "prelude.hpp"

namespace ax
{
    // Represents a value that may be missing for semantically-valid reasons.
    template<typename T>
    struct option final
    {
    public:

        CONSTRAINT(option);
        using value_type = T;
        template<typename A> using reify = ax::option<A>;
        
        option() : is_some_flag(false) { new (&u) ax::unit(); }
        
        option(const option& that) : is_some_flag(that.is_some_flag)
        {
            if (is_some_flag) new (&u) T(that.u.content);
            else new (&u) ax::unit();
        }
        
        option(option&& that) : is_some_flag(that.is_some_flag)
        {
            if (is_some_flag) new (&u) T(std::move(that.u.content));
            else new (&u) ax::unit();
        }

        option& operator=(const option& that)
        {
            if (is_some_flag) u.content.T::~T();
            else u.unit.unit::~unit();
            is_some_flag = that.is_some_flag;
            if (is_some_flag) new (&u) T(that.u.content);
            else new (&u) ax::unit();
            return *this;
        }

        option& operator=(option&& that)
        {
            if (is_some_flag) u.content.T::~T();
            else u.unit.unit::~unit();
            is_some_flag = that.is_some_flag;
            if (is_some_flag) new (&u) T(std::move(that.u.content));
            else new (&u) ax::unit();
            return *this;
        }

        explicit option(const T& content, bool) : is_some_flag(true) { new (&u) T(content); }
        explicit option(T&& content, bool) : is_some_flag(true) { new (&u) T(content); }

        ~option()
        {
            if (is_some_flag) u.content.T::~T();
            else u.unit.unit::~unit();
        }

        const T& operator*() const
        {
            if (is_some_flag) return u.content;
            throw std::runtime_error("Cannot get invalid option content.");
        }

        T& operator*()
        {
            if (is_some_flag) return u.content;
            throw std::runtime_error("Cannot get invalid option content.");
        }

        explicit operator bool() const { return is_some_flag; }
        explicit operator bool() { return is_some_flag; }

        template<typename Sf, typename Nf>
        VAR match(Sf some_fn, Nf none_fn) const
        {
            if (is_none()) return none_fn();
            return some_fn(get_content());
        }

        template<typename Sf, typename Nf>
        VAR match(Sf some_fn, Nf none_fn)
        {
            if (is_none()) return none_fn();
            return some_fn(get_content());
        }

        bool is_some() const { return static_cast<bool>(*this); }
        bool is_none() const { return !static_cast<bool>(*this); }

        const T& get_content() const { return **this; }
        T& get_content() { return **this; }

    private:

        union union_t { T content; ax::unit unit; union_t() { } ~union_t() { } } u;
        bool is_some_flag;
    };

    template<typename T>
    bool is_some(const ax::option<T>& opt) { return opt.is_some(); }

    template<typename T>
    bool is_none(const ax::option<T>& opt) { return opt.is_none(); }

    template<typename T>
    ax::option<T> some(const T& content) { return ax::option<T>(content, false); }

    template<typename T>
    ax::option<T> some(T&& content) { return ax::option<T>(content, false); }

    template<typename T>
    ax::option<T> none() { return ax::option<T>(); }
}

#endif
