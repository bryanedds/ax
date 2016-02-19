#include "../../hpp/ax/impl/field.hpp"

namespace ax
{
    field::field(
        std::type_index type_index,
        std::size_t value_offset) :
        type_index(type_index),
        value_offset(value_offset)
    { }

    std::size_t get_value_offset(const field& field)
    {
        return field.value_offset;
    }

    std::type_index get_type_index(const field& field)
    {
        return field.type_index;
    }
}
