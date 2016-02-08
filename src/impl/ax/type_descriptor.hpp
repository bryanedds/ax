#ifndef IMPL_AX_TYPE_DESCRIPTOR_HPP
#define IMPL_AX_TYPE_DESCRIPTOR_HPP

#include <cstddef>
#include <typeindex>
#include <functional>
#include <unordered_map>
#include <memory>

#include "prelude.hpp"
#include "type.hpp"
#include "reflectable.hpp"
#include "symbol.hpp"

namespace ax
{
    class type_descriptor
    {
    private:

        static std::unordered_map<std::type_index, std::shared_ptr<type_descriptor>> type_descriptor_map;

    protected:

        friend const std::unordered_map<std::type_index, std::shared_ptr<type_descriptor>>& get_type_descriptor_map();

        template<typename T, typename D>
        friend std::shared_ptr<D> register_type_descriptor(const std::shared_ptr<D>& type_descriptor);

        friend void inspect_value_vptr(const type_descriptor& type_descriptor, const reflectable& source, const field& field, void* target_ptr);
        friend void inject_value_vptr(const type_descriptor& type_descriptor, const field& field, const void* source_ptr, reflectable& target);
        friend void read_value_vptr(const type_descriptor& type_descriptor, const symbol& source_symbol, void* target_ptr);
        friend void write_value_vptr(const type_descriptor& type_descriptor, const void* source_ptr, symbol& target_symbol);

        virtual void inspect_value_impl(const void* source_ptr, void* target_ptr) const = 0;
        virtual void inject_value_impl(const void* source_ptr, void* target_ptr) const = 0;
        virtual void read_value_impl(const symbol& source_symbol, void* target_ptr) const = 0;
        virtual void write_value_impl(const void* source_ptr, symbol& target_symbol) const = 0;

    public:

        constraint(type_descriptor);
    };

    // The alias for a type descriptor map.
    using type_descriptor_map = std::unordered_map<std::type_index, std::shared_ptr<type_descriptor>>;

    // Get the type descriptor map.
    const type_descriptor_map& get_type_descriptor_map();

    // Get the type descriptor for the given type index.
    std::shared_ptr<type_descriptor> get_type_descriptor(std::type_index type_index);

    // Register a type descriptor.
    template<typename T, typename D>
    std::shared_ptr<D> register_type_descriptor(const std::shared_ptr<D>& type_descriptor)
    {
        constrain(D, type_descriptor);
        val& type_index = std::type_index(typeid(T));
        var insertion = type_descriptor::type_descriptor_map.emplace(type_index, type_descriptor);
        if (!insertion.second) insertion.first->second = type_descriptor;
        return type_descriptor;
    }

    // Get a type descriptor for the given template argument.
    template<typename T>
    std::shared_ptr<type_descriptor> get_type_descriptor()
    {
        val& type_index = std::type_index(typeid(T));
        return get_type_descriptor(type_index);
    }

    // Assign a value via void ptrs.
    template<typename T>
    void assign_value_vptr(const void* source_ptr, void* target_ptr)
    {
        val* source_ptr_t = static_cast<const T*>(source_ptr);
        var* target_ptr_t = static_cast<T*>(target_ptr);
        *target_ptr_t = *source_ptr_t;
    }

    // Assign a unique_ptr via void ptrs.
    template<typename T>
    void assign_unique_ptr_vptr(const void* source_ptr, void* target_ptr)
    {
        //constrain_as_unique_ptr(T);
        val* source_ptr_t = static_cast<const T*>(source_ptr);
        var* target_ptr_t = static_cast<T*>(target_ptr);
        target_ptr_t->reset(new typename T::element_type(**source_ptr_t));
    }

    // Assign a shared_ptr via void ptrs.
    template<typename T>
    void assign_shared_ptr_vptr(const void* source_ptr, void* target_ptr)
    {
        //constrain_as_shared_ptr(T);
        val* source_ptr_t = static_cast<const T*>(source_ptr);
        var* target_ptr_t = static_cast<T*>(target_ptr);
        target_ptr_t->reset(new typename T::element_type(**source_ptr_t));
    }

    // Inspect a value of an reflectable value, placing it into a void ptr.
    void inspect_value_vptr(const type_descriptor& type_descriptor, const reflectable& source, const field& field, void* target_ptr);

    // Inject a value into an reflectable value, via a void ptr.
    void inject_value_vptr(const type_descriptor& type_descriptor, const field& field, const void* source_ptr, reflectable& target);

    // Inspect a value of an reflectable value.
    template<typename T>
    void inspect_value(const reflectable& source, const field& field, T& target)
    {
        val& type_index = get_type_index(field);
        if (type_index != std::type_index(typeid(T))) throw std::invalid_argument("Field is not of required type.");
        val& type_descriptor = get_type_descriptor(type_index);
        var* target_ptr = static_cast<void*>(&target);
        inspect_value_vptr(*type_descriptor, source, field, target_ptr);
    }

    // Inject a value into an reflectable value.
    template<typename T>
    void inject_value(const field& field, const T& source, reflectable& target)
    {
        val& type_index = get_type_index(field);
        if (type_index != std::type_index(typeid(T))) throw std::invalid_argument("Field is not of required type.");
        val& type_descriptor = get_type_descriptor(type_index);
        val* source_ptr = static_cast<const void*>(&source);
        inject_value_vptr(*type_descriptor, field, source_ptr, target);
    }

    // Read a value from a symbol, placing it into a void ptr.
    void read_value_vptr(const type_descriptor& type_descriptor, const symbol& source_symbol, void* target_ptr);

    // Write a value to a symbol, via a void ptr.
    void write_value_vptr(const type_descriptor& type_descriptor, const void* source_ptr, symbol& target_symbol);

    // Read an reflectable value from a symbol.
    void read_value(const symbol& source_symbol, reflectable& target_reflectable);

    // Write an reflectable value to a symbol.
    void write_value(const reflectable& source_reflectable, symbol& target_symbol);

    // A generalized type descriptor for reflectable types.
    class reflectable_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override;
        void inject_value_impl(const void* source_ptr, void* target_ptr) const override;
        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override;
        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override;
    };

    // Register the common type descriptors.
    // NOTE: this is actually defined in type_descriptors.cpp, but exposed here so that the large
    // type_descriptors.hpp need not be included to access this.
    void register_common_type_descriptors();
}

#endif
