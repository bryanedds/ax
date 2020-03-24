#ifndef AX_CASTABLE_HPP
#define AX_CASTABLE_HPP

#include <cstddef>
#include <typeinfo>
#include <typeindex>
#include <memory>

#include "prelude.hpp"

namespace ax
{
    // A mixin for enabling down casts without resorting to inefficient dynamic_casts.
    // Does NOT work with multiple implementation inheritance, so you must NOT use multiple
    // implementation inheritance with any type that inherits from ax::castable.
    class castable
    {
    public:

        CONSTRAINT(castable);

        castable() = default;
        castable(const ax::castable&) = default;
        castable(ax::castable&&) = default;
        virtual ~castable() = default;
        ax::castable& operator=(const ax::castable& castable) = default;
        ax::castable& operator=(ax::castable&& castable) = default;

    protected:

        virtual const void* try_cast(std::type_index type_index) const
        {
            if (type_index == std::type_index(typeid(ax::castable))) return static_cast<const void*>(this);
            return nullptr;
        }

        virtual void* try_cast(std::type_index type_index)
        {
            if (type_index == std::type_index(typeid(ax::castable))) return static_cast<void*>(this);
            return nullptr;
        }

        template<typename T>
        friend const T* try_cast(const ax::castable& castable);

        template<typename T>
        friend T* try_cast(ax::castable& castable);
    };
}

#define ENABLE_CAST(t, s) \
    \
    const void* try_cast(std::type_index type_index) const override \
    { \
        if (type_index == std::type_index(typeid(t))) return static_cast<const void*>(this); \
        return s::try_cast(type_index); \
    } \
    \
    void* try_cast(std::type_index type_index) override \
    { \
        if (type_index == std::type_index(typeid(t))) return static_cast<void*>(this); \
        return s::try_cast(type_index); \
    } \
    \
    struct enable_cast_macro_end

#endif
