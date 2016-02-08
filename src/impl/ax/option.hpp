#ifndef IMPL_AX_OPTION_HPP
#define IMPL_AX_OPTION_HPP

#include <cstddef>
#include <stdexcept>

namespace ax
{
    // Represents a value that may be missing for semantically-valid reasons.
    template<typename T>
    class option
    {
    private:

        bool is_some;
        T content;

    public:

        constraint(option);
        using value_type = T;
        template<typename A>
        using reify = option<A>;
        
        option() : is_some(false) { }
        option(const option&) = default;
        option(option&&) = default;
        option& operator=(const option&) = default;
        option& operator=(option&&) = default;
        ~option() = default;

        explicit option(const T& content, bool) : is_some(true), content(content) { }
        explicit option(T&& content, bool) : is_some(true), content(content) { }
        explicit operator bool() const { return is_some; }
        explicit operator bool() { return is_some; }

        const T& operator*() const
        {
            if (is_some) return content;
            throw std::runtime_error("Cannot get invalid option content.");
        }

        T& operator*()
        {
            if (is_some) return content;
            throw std::runtime_error("Cannot get invalid option content.");
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
    Var match(const O& opt, Sf some_fn, Nf none_fn)
    {
        constrain(O, option);
        if (is_none(opt)) return none_fn();
        return some_fn(get_content(opt));
    }

    template<typename O, typename Sf, typename Nf>
    Var match(O& opt, Sf some_fn, Nf none_fn)
    {
        constrain(O, option);
        if (is_none(opt)) return none_fn();
        return some_fn(get_content(opt));
    }
}

#endif
