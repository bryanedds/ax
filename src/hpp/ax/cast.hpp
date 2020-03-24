#ifndef AX_CASTING_HPP
#define AX_CASTING_HPP

#include <cstddef>
#include <typeinfo>
#include <typeindex>
#include <memory>

#include "castable.hpp"

namespace ax
{
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

#endif
