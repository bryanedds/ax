#include <vector>

#include "../../hpp/ax/impl/type_descriptor.hpp"

#include "../../hpp/ax/impl/string.hpp"

namespace ax
{
    /* type_descriptor */

    type_descriptor_map type_descriptor::type_descriptor_map;

    void type_descriptor::inspect_value(const reflectable& source, const field& field, void* target_ptr) const
    {
        VAR* source_ptr = &source;
        VAR* head_ptr = static_cast<const char*>(static_cast<const void*>(source_ptr));
        VAR* field_ptr = static_cast<const void*>(head_ptr + field.get_value_offset());
        inspect_value(field_ptr, target_ptr);
    }

    void type_descriptor::inject_value(const field& field, const void* source_ptr, reflectable& target) const
    {
        VAR* target_ptr = &target;
        VAR* head_ptr = static_cast<char*>(static_cast<void*>(target_ptr));
        VAR* field_ptr = static_cast<void*>(head_ptr + field.get_value_offset());
        inject_value(source_ptr, field_ptr);
    }

    const ax::type_descriptor_map& get_type_descriptor_map()
    {
        return ax::type_descriptor::type_descriptor_map;
    }

    std::shared_ptr<ax::type_descriptor> get_type_descriptor(std::type_index type_index)
    {
        VAL& type_descriptor_map = ax::get_type_descriptor_map();
        VAL& type_descriptor_iter = type_descriptor_map.find(type_index);
        if (type_descriptor_iter == std::end(type_descriptor_map))
        {
            VAL& type_map = ax::get_type_map();
            VAL& type_iter = type_map.find(type_index);
            if (type_iter == std::end(type_map)) throw std::runtime_error("Could not find type descriptor for '"_s + type_index.name() + "'.");
            return std::make_shared<ax::reflectable_descriptor>();
        }
        return type_descriptor_iter->second;
    }

    void read_value(const ax::symbol& source_symbol, ax::reflectable& target_reflectable)
    {
        VAL& type_descriptor = ax::get_type_descriptor<ax::reflectable>();
        type_descriptor->read_value(source_symbol, static_cast<void*>(&target_reflectable));
    }

    void write_value(const ax::reflectable& source_reflectable, ax::symbol& target_symbol)
    {
        VAL& type = ax::get_type(source_reflectable);
        VAL& type_index = type->get_type_index();
        VAL& type_descriptor = ax::get_type_descriptor(type_index);
        type_descriptor->write_value(static_cast<const void*>(&source_reflectable), target_symbol);
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

    static void read_value_internal(const std::shared_ptr<type_t>& type, const symbols_t& symbols, ax::reflectable& reflectable)
    {
        // read sub-type values
        if (VAL* base_type_index = type->get_base_type_index_opt().get())
        {
            VAL& base_type = ax::get_type(*base_type_index);
            ax::read_value_internal(base_type, symbols, reflectable);
        }

        // read current type values
        VAL& field_vector = type->get_field_vector();
        VAR symbols_iter = std::begin(symbols);
        VAL symbols_end = std::end(symbols);
        for (VAL& field_kvp : field_vector)
        {
            if (symbols_iter != symbols_end)
            {
                VAL& field = field_kvp.second;
                VAL& field_symbol = *symbols_iter;
                VAL& field_type_index = field->get_type_index();
                VAL& field_type_descriptor = ax::get_type_descriptor(field_type_index);
                VAL& field_ptr = static_cast<char*>(static_cast<void*>(&reflectable)) + field->get_value_offset();
                field_type_descriptor->read_value(field_symbol, field_ptr);
                ++symbols_iter;
            }
        }
    }

    void write_value_internal(const std::shared_ptr<type_t>& type, const ax::reflectable& reflectable, ax::symbols_t& symbols)
    {
        // write sub-type values
        if (VAL* base_type_index = type->get_base_type_index_opt().get())
        {
            VAL& base_type = get_type(*base_type_index);
            write_value_internal(base_type, reflectable, symbols);
        }

        // write current type values
        VAL& field_vector = type->get_field_vector();
        for (VAL& field_kvp : field_vector)
        {
            ax::symbol field_symbol_mvb{};
            VAL& field = field_kvp.second;
            VAL& field_type_index = field->get_type_index();
            VAL& field_type_descriptor = ax::get_type_descriptor(field_type_index);
            VAL& field_ptr = static_cast<const char*>(static_cast<const void*>(&reflectable)) + field->get_value_offset();
            field_type_descriptor->write_value(field_ptr, field_symbol_mvb);
            symbols.emplace_back(std::move(field_symbol_mvb));
        }
    }

    void reflectable_descriptor::write_value(const void* source_ptr, ax::symbol& target_symbol) const
    {
        // ensure target is a symbols value
        if (!target_symbol.is_symbols()) target_symbol = ax::symbols({});

        // get type to write
        VAR& symbols = target_symbol.get_symbols();
        VAL* reflectable_ptr = static_cast<const ax::reflectable*>(source_ptr);
        VAL& reflectable = *reflectable_ptr;
        VAL& type = ax::get_type(reflectable);

        // write values
        ax::write_value_internal(type, reflectable, symbols);
    }

    void reflectable_descriptor::read_value(const ax::symbol& source_symbol, void* target_ptr) const
    {
        VAR* reflectable_ptr = static_cast<ax::reflectable*>(target_ptr);
        source_symbol.match(
        [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
        [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
        [&](VAL& symbols)
        {
            VAR& reflectable = *reflectable_ptr;
            VAL& type = ax::get_type(reflectable);
            ax::read_value_internal(type, symbols, reflectable);
        });
    }
}
