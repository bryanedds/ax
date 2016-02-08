#ifndef IMPL_AX_TYPE_HPP
#define IMPL_AX_TYPE_HPP

#include <cstddef>
#include <typeindex>
#include <initializer_list>
#include <memory>
#include <unordered_map>

#include "prelude.hpp"
#include "name.hpp"
#include "field.hpp"

namespace ax
{
    class type_t
    {
    private:

        static std::unordered_map<std::type_index, std::shared_ptr<type_t>> type_map;
        const std::shared_ptr<std::type_index> base_type_index_opt;
        const std::type_index type_index;
        const field_map field_map;
        const field_vector field_vector;

    protected:
        
        template<typename T>
        friend std::shared_ptr<type_t> register_any_type(std::shared_ptr<std::type_index>, std::initializer_list<std::pair<name_t, field>> field_init_list);
        friend const std::unordered_map<std::type_index, std::shared_ptr<type_t>>& get_type_map();
        friend const std::shared_ptr<std::type_index>& get_base_type_index_opt(const type_t& type);
        friend std::type_index get_type_index(const type_t& type);
        friend const ax::field_map& get_field_map(const type_t& type);
        friend const ax::field_vector& get_field_vector(const type_t& type);

    public:

        type_t(
            const std::shared_ptr<std::type_index>& base_type_index_opt,
            std::type_index type_index,
            const ax::field_map& field_map,
            const ax::field_vector& field_vector);
    };

    // The alias for a type map.
    using type_map = std::unordered_map<std::type_index, std::shared_ptr<type_t>>;

    // Get the type map.
    const type_map& get_type_map();

    // Get the type with the given type index.
    std::shared_ptr<type_t> get_type(std::type_index type_index);

    // Get the type index of the base type, if any.
    const std::shared_ptr<std::type_index>& get_base_type_index_opt(const type_t& type);

    // Get the type index of a type.
    std::type_index get_type_index(const type_t& type);

    // Get the field map of a type.
    const field_map& get_field_map(const type_t& type);

    // Get the field vector of a type.
    const field_vector& get_field_vector(const type_t& type);

    // Register a type for reflection that has a base type.
    template<typename T>
    std::shared_ptr<type_t> register_any_type(std::shared_ptr<std::type_index> base_type_index_opt, std::initializer_list<std::pair<name_t, field>> field_init_list)
    {
        field_vector field_vector{};
        for (val& field_kvp : field_init_list) field_vector.emplace_back(std::make_pair(field_kvp.first, std::make_shared<field>(field_kvp.second)));
        field_map field_map{};
        for (val& field_kvp : field_vector) field_map.insert(field_kvp);
        val& type_index = std::type_index(typeid(T));
        val& type = std::make_shared<type_t>(base_type_index_opt, type_index, field_map, field_vector);
        type_t::type_map.insert(std::make_pair(type_index, type));
        return type;
    }

    // Register a type for reflection.
    template<typename T>
    std::shared_ptr<type_t> register_sub_type(const std::type_info& base_type_info, std::initializer_list<std::pair<name_t, field>> field_init_list)
    {
        val& base_type_index_ptr = std::make_shared<std::type_index>(base_type_info);
        return register_any_type<T>(base_type_index_ptr, field_init_list);
    }

    // Register a type for reflection.
    template<typename T>
    std::shared_ptr<type_t> register_type(std::initializer_list<std::pair<name_t, field>> field_init_list)
    {
        return register_any_type<T>(std::shared_ptr<std::type_index>(), field_init_list);
    }
}

#endif
