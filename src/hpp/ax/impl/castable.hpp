#ifndef IMPL_AX_CASTABLE_HPP
#define IMPL_AX_CASTABLE_HPP

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

    template<typename T>
    const T* try_cast(const ax::castable& castable)
    {
        VAL type_index = std::type_index(typeid(T));
        return static_cast<const T*>(castable.try_cast(type_index));
    }

    template<typename T>
    T* try_cast(ax::castable& castable)
    {
        VAL type_index = std::type_index(typeid(T));
        return static_cast<T*>(castable.try_cast(type_index));
    }

    template<typename T>
    const T& cast(const ax::castable& castable)
    {
        return *try_cast<T>(castable);
    }

    template<typename T>
    T& cast(ax::castable& castable)
    {
        return *try_cast<T>(castable);
    }

    template<typename T>
    const T* try_cast(const ax::castable* castable)
    {
        return try_cast<T>(*castable);
    }

    template<typename T>
    T* try_cast(ax::castable* castable)
    {
        return try_cast<T>(*castable);
    }

    template<typename T>
    const T* cast(const ax::castable* castable)
    {
        return &cast<T>(*castable);
    }

    template<typename T>
    T* cast(ax::castable* castable)
    {
        return &cast<T>(*castable);
    }

    template<typename U, typename T>
    std::shared_ptr<U> try_cast(const std::shared_ptr<T>& source)
    {
        VAR u_opt = try_cast<U>(*source);
        if (u_opt) return std::shared_ptr<U>(source, u_opt);
        return std::shared_ptr<U>();
    }

    template<typename U, typename T>
    std::shared_ptr<U> cast(const std::shared_ptr<T>& source)
    {
        VAR u_opt = try_cast<U>(*source);
        if (u_opt) return std::shared_ptr<U>(source, u_opt);
        throw std::logic_error("Invalid cast.");
    }

    template<typename U, typename T>
    std::unique_ptr<U> cast(std::unique_ptr<T>&& source)
    {
        VAR u_opt = try_cast<U>(*source);
        if (u_opt)
        {
            source.release();
            return std::unique_ptr<U>(u_opt);
        }
        throw std::logic_error("Invalid cast.");
    }
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
