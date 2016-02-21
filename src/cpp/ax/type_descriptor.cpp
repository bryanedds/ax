#include <vector>

#include "../../hpp/ax/impl/type_descriptor.hpp"

#include "../../hpp/ax/impl/string.hpp"

namespace ax
{
    /* type_descriptor */

    type_descriptor_map type_descriptor::type_descriptor_map;

    const type_descriptor_map& get_type_descriptor_map()
    {
        return type_descriptor::type_descriptor_map;
    }

    std::shared_ptr<type_descriptor> get_type_descriptor(std::type_index type_index)
    {
        VAL& type_descriptor_map = get_type_descriptor_map();
        VAL& type_descriptor_iter = type_descriptor_map.find(type_index);
        if (type_descriptor_iter == std::end(type_descriptor_map))
        {
            VAL& type_map = get_type_map();
            VAL& type_iter = type_map.find(type_index);
            if (type_iter == std::end(type_map)) throw std::runtime_error("Could not find type descriptor for '"_s + type_index.name() + "'.");
            return std::make_shared<reflectable_descriptor>();
        }
        return type_descriptor_iter->second;
    }

    void inspect_value_vptr(const type_descriptor& type_descriptor, const reflectable& source, const field& field, void* target_ptr)
    {
        VAR* source_ptr = &source;
        VAR* head_ptr = static_cast<const char*>(static_cast<const void*>(source_ptr));
        VAR* field_ptr = static_cast<const void*>(head_ptr + get_value_offset(field));
        type_descriptor.inspect_value(field_ptr, target_ptr);
    }

    void inject_value_vptr(const type_descriptor& type_descriptor, const field& field, const void* source_ptr, reflectable& target)
    {
        VAR* target_ptr = &target;
        VAR* head_ptr = static_cast<char*>(static_cast<void*>(target_ptr));
        VAR* field_ptr = static_cast<void*>(head_ptr + get_value_offset(field));
        type_descriptor.inject_value(source_ptr, field_ptr);
    }

    void read_value_vptr(const type_descriptor& type_descriptor, const symbol& source_symbol, void* target_ptr)
    {
        type_descriptor.read_value(source_symbol, target_ptr);
    }

    void write_value_vptr(const type_descriptor& type_descriptor, const void* source_ptr, symbol& target_symbol)
    {
        type_descriptor.write_value(source_ptr, target_symbol);
    }

    void read_value(const symbol& source_symbol, reflectable& target_reflectable)
    {
        VAL& type_descriptor = get_type_descriptor<reflectable>();
        read_value_vptr(*type_descriptor, source_symbol, static_cast<void*>(&target_reflectable));
    }

    void write_value(const reflectable& source_reflectable, symbol& target_symbol)
    {
        VAL& type = get_type(source_reflectable);
        VAL& type_index = get_type_index(*type);
        VAL& type_descriptor = get_type_descriptor(type_index);
        write_value_vptr(*type_descriptor, static_cast<const void*>(&source_reflectable), target_symbol);
    }

    /* reflectable_descriptor */

    void reflectable_descriptor::inspect_value(const void*, void*) const
    {
        throw std::runtime_error("reflectable_descriptor::inspect_value not implemented.");
    }

    void reflectable_descriptor::inject_value(const void*, void*) const
    {
        throw std::runtime_error("reflectable_descriptor::inject_value not implemented.");
    }

    static void read_value_internal(const std::shared_ptr<type_t>& type, const symbols_t& symbols, reflectable& reflectable)
    {
        // read sub-type values
        if (VAL* base_type_index = get_base_type_index_opt(*type).get())
        {
            VAL& base_type = get_type(*base_type_index);
            read_value_internal(base_type, symbols, reflectable);
        }

        // read current type values
        VAL& field_vector = get_field_vector(*type);
        VAR symbols_iter = std::begin(symbols);
        VAL symbols_end = std::end(symbols);
        for (VAL& field_kvp : field_vector)
        {
            if (symbols_iter != symbols_end)
            {
                VAL& field = field_kvp.second;
                VAL& field_symbol = *symbols_iter;
                VAL& field_type_index = get_type_index(*field);
                VAL& field_type_descriptor = get_type_descriptor(field_type_index);
                VAL& field_ptr = static_cast<char*>(static_cast<void*>(&reflectable)) + get_value_offset(*field);
                read_value_vptr(*field_type_descriptor, field_symbol, field_ptr);
                ++symbols_iter;
            }
        }
    }

    void write_value_internal(const std::shared_ptr<type_t>& type, const reflectable& reflectable, symbols_t& symbols)
    {
        // write sub-type values
        if (VAL* base_type_index = get_base_type_index_opt(*type).get())
        {
            VAL& base_type = get_type(*base_type_index);
            write_value_internal(base_type, reflectable, symbols);
        }

        // write current type values
        VAL& field_vector = get_field_vector(*type);
        for (VAL& field_kvp : field_vector)
        {
            symbol field_symbol_mvb{};
            VAL& field = field_kvp.second;
            VAL& field_type_index = get_type_index(*field);
            VAL& field_type_descriptor = get_type_descriptor(field_type_index);
            VAL& field_ptr = static_cast<const char*>(static_cast<const void*>(&reflectable)) + get_value_offset(*field);
            write_value_vptr(*field_type_descriptor, field_ptr, field_symbol_mvb);
            symbols.emplace_back(std::move(field_symbol_mvb));
        }
    }

    void reflectable_descriptor::write_value(const void* source_ptr, symbol& target_symbol) const
    {
        // ensure target is a symbols value
        if (!is_symbols(target_symbol)) target_symbol = symbols({});

        // get type to write
        VAR& symbols = get_symbols(target_symbol);
        VAL* reflectable_ptr = static_cast<const reflectable*>(source_ptr);
        VAL& reflectable = *reflectable_ptr;
        VAL& type = get_type(reflectable);

        // write values
        write_value_internal(type, reflectable, symbols);
    }

    void reflectable_descriptor::read_value(const symbol& source_symbol, void* target_ptr) const
    {
        VAR* reflectable_ptr = static_cast<reflectable*>(target_ptr);
        match3(source_symbol,
        [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
        [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
        [&](VAL& symbols)
        {
            VAR& reflectable = *reflectable_ptr;
            VAL& type = get_type(reflectable);
            read_value_internal(type, symbols, reflectable);
        });
    }
}
