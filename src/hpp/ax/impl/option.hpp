#ifndef IMPL_AX_OPTION_HPP
#define IMPL_AX_OPTION_HPP

#include <cstddef>
#include <stdexcept>

#include "prelude.hpp"

namespace ax
{
    // Represents a value that may be missing for semantically-valid reasons.
    template<typename T>
    class option
    {
    private:

        union union_t { T content; ax::unit unit; union_t() { } ~union_t() { } } u;
        bool is_some;

    public:

        CONSTRAINT(option);
        using value_type = T;
        template<typename A>
        using reify = option<A>;
        
        option() : is_some(false)
        {
            new (&u.unit) unit();
        }
        
        option(const option& that) : is_some(that.is_some)
        {
            if (is_some) new (&u.content) T(that.u.content);
            else new (&u.unit) unit();
        }
        
        option(option&& that) : is_some(that.is_some)
        {
            if (is_some) new (&u.content) T(std::move(that.u.content));
            else new (&u.unit) unit();
        }

        option& operator=(const option& that)
        {
            is_some = that.is_some;
            if (is_some) u.content = that.u.content;
            else u.unit = that.u.unit;
            return *this;
        }

        option& operator=(option&& that)
        {
            is_some = that.is_some;
            if (is_some) u.content = std::move(that.u.content);
            else u.unit = that.u.unit;
            return *this;
        }

        explicit option(const T& content, bool) : is_some(true)
        {
            new (&u.content) T(content);
        }

        explicit option(T&& content, bool) : is_some(true)
        {
            new (&u.content) T(content);
        }

        ~option()
        {
            if (is_some) u.content.T::~T();
            else u.unit.unit::~unit();
        }

        const T& operator*() const
        {
            if (is_some) return u.content;
            throw std::runtime_error("Cannot get invalid option content.");
        }

        T& operator*()
        {
            if (is_some) return u.content;
            throw std::runtime_error("Cannot get invalid option content.");
        }

        explicit operator bool() const
        {
            return is_some;
        }

        explicit operator bool()
        {
            return is_some;
        }
    };

    template<typename T>
    bool is_some(const option<T>& opt)
    {
        return static_cast<bool>(opt);
    }

    template<typename T>
    bool is_none(const option<T>& opt)
    {
        return !static_cast<bool>(opt);
    }

    template<typename T>
    const T& get_content(const option<T>& opt)
    {
        return *opt;
    }

    template<typename T>
    T& get_content(option<T>& opt)
    {
        return *opt;
    }

    template<typename T>
    option<T> some(const T& content)
    {
        return option<T>(content, false);
    }

    template<typename T>
    option<T> some(T&& content)
    {
        return option<T>(content, false);
    }

    template<typename T>
    option<T> none()
    {
        return option<T>();
    }

    template<typename O, typename Sf, typename Nf>
    VAR match_opt(const O& opt, Sf some_fn, Nf none_fn)
    {
        CONSTRAIN(O, option);
        if (is_none(opt)) return none_fn();
        return some_fn(get_content(opt));
    }

    template<typename O, typename Sf, typename Nf>
    VAR match_opt(O& opt, Sf some_fn, Nf none_fn)
    {
        CONSTRAIN(O, option);
        if (is_none(opt)) return none_fn();
        return some_fn(get_content(opt));
    }
}

#endif
