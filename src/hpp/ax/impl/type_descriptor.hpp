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
    public:

        CONSTRAINT(type_descriptor);

    protected:

        friend const std::unordered_map<std::type_index, std::shared_ptr<type_descriptor>>& get_type_descriptor_map();

        template<typename T, typename D>
        friend std::shared_ptr<D> register_type_descriptor(const std::shared_ptr<D>& type_descriptor);

        friend void inspect_value(const type_descriptor& type_descriptor, const reflectable& source, const field& field, void* target_ptr);
        friend void inject_value(const type_descriptor& type_descriptor, const field& field, const void* source_ptr, reflectable& target);
        friend void read_value(const type_descriptor& type_descriptor, const symbol& source_symbol, void* target_ptr);
        friend void write_value(const type_descriptor& type_descriptor, const void* source_ptr, symbol& target_symbol);

        virtual void inspect_value(const void* source_ptr, void* target_ptr) const = 0;
        virtual void inject_value(const void* source_ptr, void* target_ptr) const = 0;
        virtual void read_value(const symbol& source_symbol, void* target_ptr) const = 0;
        virtual void write_value(const void* source_ptr, symbol& target_symbol) const = 0;

    private:

        static std::unordered_map<std::type_index, std::shared_ptr<type_descriptor>> type_descriptor_map;
    };

    // The alias for a type descriptor map.
    using type_descriptor_map = std::unordered_map<std::type_index, std::shared_ptr<type_descriptor>>;

    // Get the type descriptor map.
    const type_descriptor_map& get_type_descriptor_map();

    // Get the type descriptor for the given type index.
    std::shared_ptr<type_descriptor> get_type_descriptor(std::type_index type_index);

    // Get a type descriptor for the given template argument.
    template<typename T>
    std::shared_ptr<type_descriptor> get_type_descriptor()
    {
        VAL& type_index = std::type_index(typeid(T));
        return get_type_descriptor(type_index);
    }

    // Register a type descriptor.
    template<typename T, typename D>
    std::shared_ptr<D> register_type_descriptor(const std::shared_ptr<D>& type_descriptor)
    {
        CONSTRAIN(D, type_descriptor);
        VAL& type_index = std::type_index(typeid(T));
        VAR insertion = type_descriptor::type_descriptor_map.emplace(type_index, type_descriptor);
        if (!insertion.second) insertion.first->second = type_descriptor;
        return type_descriptor;
    }

    // Assign a value via void ptrs.
    template<typename T>
    void assign_value(const void* source_ptr, void* target_ptr)
    {
        VAL* source_ptr_t = static_cast<const T*>(source_ptr);
        VAR* target_ptr_t = static_cast<T*>(target_ptr);
        *target_ptr_t = *source_ptr_t;
    }

    // Assign a shared_ptr via void ptrs.
    template<typename T>
    void assign_shared_ptr(const void* source_ptr, void* target_ptr)
    {
        CONSTRAIN_AS_SHARED_PTR(T);
        VAL* source_ptr_ptr = static_cast<const std::shared_ptr<T>*>(source_ptr);
        VAR* target_ptr_ptr = static_cast<std::shared_ptr<T>*>(target_ptr);
        *target_ptr_ptr = *source_ptr_ptr;
    }

    // Assign a unique_ptr via void ptrs.
    template<typename T>
    void assign_unique_ptr(void* source_ptr, void* target_ptr)
    {
        CONSTRAIN_AS_UNIQUE_PTR(T);
        VAR* source_ptr_ptr = static_cast<std::unique_ptr<T>*>(source_ptr);
        VAR* target_ptr_ptr = static_cast<std::unique_ptr<T>*>(target_ptr);
        target_ptr_ptr->reset(source_ptr_ptr->get());
    }

    // Inspect a value of a reflectable value, placing it into a void ptr.
    void inspect_value(const type_descriptor& type_descriptor, const reflectable& source, const field& field, void* target_ptr);

    // Inject a value into a reflectable value, via a void ptr.
    void inject_value(const type_descriptor& type_descriptor, const field& field, const void* source_ptr, reflectable& target);

    // Inspect a value of a reflectable value.
    template<typename T>
    void inspect_value(const reflectable& source, const field& field, T& target)
    {
        VAL& type_index = get_type_index(field);
        if (type_index != std::type_index(typeid(T))) throw std::invalid_argument("Field is not of required type.");
        VAL& type_descriptor = get_type_descriptor(type_index);
        VAR* target_ptr = static_cast<void*>(&target);
        inspect_value(*type_descriptor, source, field, target_ptr);
    }

    // Inject a value into a reflectable value.
    template<typename T>
    void inject_value(const field& field, const T& source, reflectable& target)
    {
        VAL& type_index = get_type_index(field);
        if (type_index != std::type_index(typeid(T))) throw std::invalid_argument("Field is not of required type.");
        VAL& type_descriptor = get_type_descriptor(type_index);
        VAL* source_ptr = static_cast<const void*>(&source);
        inject_value(*type_descriptor, field, source_ptr, target);
    }

    // Read a value from a symbol, placing it into a void ptr.
    void read_value(const type_descriptor& type_descriptor, const symbol& source_symbol, void* target_ptr);

    // Read a reflectable value from a symbol.
    void read_value(const symbol& source_symbol, reflectable& target_reflectable);

    // Write a value to a symbol, via a void ptr.
    void write_value(const type_descriptor& type_descriptor, const void* source_ptr, symbol& target_symbol);

    // Write a reflectable value to a symbol.
    void write_value(const reflectable& source_reflectable, symbol& target_symbol);

    // A generalized type descriptor for reflectable types.
    class reflectable_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, symbol& target_symbol) const override;
    };

    // Register the common type descriptors.
    // NOTE: this is actually defined in type_descriptors.cpp, but exposed here so that the large
    // type_descriptors.hpp need not be included to access this.
    void register_common_type_descriptors();
}

#endif
