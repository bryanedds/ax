#ifndef AX_FIELD_HPP
#define AX_FIELD_HPP

#include <cstddef>
#include <typeindex>
#include <memory>
#include <unordered_map>

#include "prelude.hpp"
#include "name.hpp"

namespace ax
{
    class field
    {
    private:

        const std::type_index type_index;
        const std::size_t value_offset;

    protected:

        friend std::size_t get_value_offset(const field& field);
        friend std::type_index get_type_index(const field& field);

    public:

        field(std::type_index type_index, std::size_t value_offset);
    };

    // The alias for a field map.
    using field_map = std::unordered_map<name_t, std::shared_ptr<field>>;

    // The alias for a field vector.
    using field_vector = std::vector<std::pair<name_t, std::shared_ptr<field>>>;

    // Register a field for inspection.
    template<typename T>
    field register_field(std::size_t value_offset)
    {
        val& type_index = std::type_index(typeid(T));
        return field(type_index, value_offset);
    }

    // Get the value offset of a field.
    std::size_t get_value_offset(const field& field);

    // Get the type index of a field.
    std::type_index get_type_index(const field& field);
}

#endif
