#ifndef IMPL_AX_PRELUDE_HPP
#define IMPL_AX_PRELUDE_HPP

// General Note: This library target C++14-conformant (or near-conformant) compilers.
//
// The newer C++ features from which this library would greatly benefit are that of Concepts,
// Uniform Call Syntax, and Filesystem. Modules would also be beneficiary, but to a limited extent
// due to the small size of this library and its dependencies.

#include <cstddef>
#include <type_traits>

// Variable shadowing is a good thing when doing functional-style programming.
#if defined(_MSC_VER)
    #pragma warning(disable: 4456)
    #pragma warning(disable: 4457)
    #pragma warning(disable: 4458)
#elif defined(__CLANG__)
    #pragma clang diagnostic ignored "-Wno-vexing-parse"
#endif

// Use of unconditional branches is a common technique in C++ metaprogramming.
#if defined(_MSC_VER)
    #pragma warning(disable: 4127)
#endif

// Short-hand for immutable auto.
#define VAL auto const

// Short-hand for mutable auto.
#define VAR auto

// Declare a type to be constraint.
#define CONSTRAINT(constraint_name) \
    using constraint_name##_constraint = void

// Assert a type constraint.
#define CONSTRAIN(type, constraint_name) \
    do { \
        struct unique_type; \
        static_assert(!std::is_same<typename type::constraint_name##_constraint, unique_type>::value, #type " needs to satisfy " #constraint_name "."); \
    } while (false)

// Assert a type is a container.
#define CONSTRAIN_AS_CONTAINER(type) \
    do { \
        struct unique_type; \
        static_assert(!std::is_same<typename type::size_type, unique_type>::value, #type " needs to be a container."); \
    } while (false)

// Assert a type is a unique ptr.
#define CONSTRAIN_AS_UNIQUE_PTR(type) \
    do { \
        struct unique_type; \
        static_assert(!std::is_same<typename type::element_type, unique_type>::value, #type " needs to be a unique_ptr."); \
    } while (false)

// Assert a type is a shared ptr.
#define CONSTRAIN_AS_SHARED_PTR(type) \
    do { \
        struct unique_type; \
        static_assert(!std::is_same<typename type::element_type, unique_type>::value, #type " needs to be a shared_ptr."); \
    } while (false)

// Enable std::size_t literals.
// Hopefully can be replaced with a built-in operator""z soon.
constexpr std::size_t operator""_z(unsigned long long n)
{
    return static_cast<std::size_t>(n);
}

namespace ax
{
    // Alias for std::size_t (not currently used).
    using sz = std::size_t;

    // Convert from std::size_t to int.
    constexpr int ztoi(std::size_t z) { return static_cast<int>(z); }

    // Convert from int to std::size_t.
    constexpr std::size_t itoz(int i) { return static_cast<std::size_t>(i); }

    // Zero value for a given T.
    template<typename T>
    constexpr T zero() { return static_cast<T>(0); }

    // One value for a given T.
    template<typename T>
    constexpr T one() { return static_cast<T>(1); }

    // Predecessor of a T value.
    template<typename T>
    T pred(T t) { return t - one<T>(); }

    // Successor of a T value.
    template<typename T>
    T succ(T t) { return t + one<T>(); }

    // The unit type.
    struct unit final { public: CONSTRAINT(unit); };
}

namespace std
{
    // Transform as an expression.
    // NOTE: it is not legal to add to the std namespace, but since this form is so egregiously
    // missing, I did it anyway.
    template<typename Cr, typename It, typename Fn>
    Cr transform(const It& begin, const It& end, const Fn& fn)
    {
        CONSTRAIN_AS_CONTAINER(Cr);
        Cr transformed{};
        for (VAR it = begin; it != end; ++it)
        {
            VAL& temp = fn(*it);
            transformed.push_back(temp);
        }
        return transformed;
    }
    
    // Insert or assign to a mapping container.
    // NOTE: this is only needed for clang builds.
    template<typename Map, typename T>
    auto insert_or_assign(Map& m, const typename Map::key_type& key, const T& value)
    {
        auto p = m.insert({ key, value });
        if (!p.second) p.first->second = value;
        return p;
    }
}

#endif
