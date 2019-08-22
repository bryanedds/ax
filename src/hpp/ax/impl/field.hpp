#ifndef IMPL_AX_FIELD_HPP
#define IMPL_AX_FIELD_HPP

#include <cstddef>
#include <typeindex>
#include <memory>
#include <vector>
#include <unordered_map>

#include "prelude.hpp"
#include "name.hpp"

namespace ax
{
    class field
    {
    public:

        field(std::type_index type_index, std::size_t value_offset);
        std::size_t get_value_offset() const;
        std::type_index get_type_index() const;

    private:

        const std::type_index type_index;
        const std::size_t value_offset;
    };

    // The alias for a field map.
    using field_map = std::unordered_map<name_t, std::shared_ptr<field>>;

    // The alias for a field vector.
    using field_vector = std::vector<std::pair<name_t, std::shared_ptr<field>>>;

    // Register a field for reflection.
    template<typename T>
    field register_field(std::size_t value_offset)
    {
        VAL& type_index = std::type_index(typeid(T));
        return field(type_index, value_offset);
    }
}

#endif
