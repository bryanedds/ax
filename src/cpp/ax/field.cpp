#include "../../hpp/ax/impl/field.hpp"

namespace ax
{
    field::field(
        std::type_index type_index,
        std::size_t value_offset) :
        type_index(type_index),
        value_offset(value_offset)
    { }

    std::size_t field::get_value_offset() const
    {
        return value_offset;
    }

    std::type_index field::get_type_index() const
    {
        return type_index;
    }
}
