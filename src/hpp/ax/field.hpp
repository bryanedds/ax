#ifndef AX_FIELD_HPP
#define AX_FIELD_HPP

#include <cstddef>
#include <typeindex>
#include <memory>
#include <unordered_map>

#include "prelude.hpp"
#include "name.hpp"
#include "vector.hpp"

namespace ax
{
    struct field final
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
    using field_map = std::unordered_map<ax::name, std::shared_ptr<ax::field>>;

    // The alias for a field vector.
    using field_vector = ax::vector<std::pair<ax::name, std::shared_ptr<ax::field>>>;

    // Register a field for reflection.
    template<typename T>
    ax::field register_field(std::size_t value_offset)
    {
        VAL& type_index = std::type_index(typeid(T));
        return ax::field(type_index, value_offset);
    }
}

#endif
