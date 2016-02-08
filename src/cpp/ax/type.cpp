#include "../../impl/ax/type.hpp"

namespace ax
{
    type_map type_t::type_map;

    type_t::type_t(
        const std::shared_ptr<std::type_index>& base_type_index_opt,
        std::type_index type_index,
        const ax::field_map& field_map,
        const ax::field_vector& field_vector) :
        base_type_index_opt(base_type_index_opt),
        type_index(type_index),
        field_map(field_map),
        field_vector(field_vector)
    { }

    const type_map& get_type_map()
    {
        return type_t::type_map;
    }

    std::shared_ptr<type_t> get_type(std::type_index type_index)
    {
        val& type_map = get_type_map();
        val& type_iter = type_map.find(type_index);
        if (type_iter == std::end(type_map)) throw std::runtime_error("Could not find type '"_s + type_index.name() + "'.");
        return type_iter->second;
    }

    const std::shared_ptr<std::type_index>& get_base_type_index_opt(const type_t& type)
    {
        return type.base_type_index_opt;
    }

    std::type_index get_type_index(const type_t& type)
    {
        return type.type_index;
    }

    const field_map& get_field_map(const type_t& type)
    {
        return type.field_map;
    }

    const field_vector& get_field_vector(const type_t& type)
    {
        return type.field_vector;
    }
}
