#include <exception>
#include <stdexcept>

#include "ax/type.hpp"

#include "ax/string.hpp"

namespace ax
{
    type_map ax::type::type_map;

    type::type(
        const std::shared_ptr<std::type_index>& base_type_index_opt,
        std::type_index type_index,
        const ax::field_map& field_map,
        const ax::field_vector& field_vector) :
        base_type_index_opt(base_type_index_opt),
        type_index(type_index),
        field_map(field_map),
        field_vector(field_vector)
    { }

    const std::shared_ptr<std::type_index>& type::get_base_type_index_opt() const
    {
        return base_type_index_opt;
    }

    std::type_index type::get_type_index() const
    {
        return type_index;
    }

    const field_map& type::get_field_map() const
    {
        return field_map;
    }

    const field_vector& type::get_field_vector() const
    {
        return field_vector;
    }

    const type_map& get_type_map()
    {
        return ax::type::type_map;
    }

    std::shared_ptr<ax::type> get_type(std::type_index type_index)
    {
        VAL& type_map = get_type_map();
        VAL& type_iter = type_map.find(type_index);
        if (type_iter == std::end(type_map)) throw std::runtime_error("Could not find type '"_s + type_index.name() + "'.");
        return type_iter->second;
    }
}
