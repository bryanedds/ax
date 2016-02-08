#include <vector>

#include "../../hpp/ax/type_descriptor.hpp"

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
        Val& type_descriptor_map = get_type_descriptor_map();
        Val& type_descriptor_iter = type_descriptor_map.find(type_index);
        if (type_descriptor_iter == std::end(type_descriptor_map))
        {
            Val& type_map = get_type_map();
            Val& type_iter = type_map.find(type_index);
            if (type_iter == std::end(type_map)) throw std::runtime_error("Could not find type descriptor for '"_s + type_index.name() + "'.");
            return std::make_shared<reflectable_descriptor>();
        }
        return type_descriptor_iter->second;
    }

    void inspect_value_vptr(const type_descriptor& type_descriptor, const reflectable& source, const field& field, void* target_ptr)
    {
        Var* source_ptr = &source;
        Var* head_ptr = static_cast<const char*>(static_cast<const void*>(source_ptr));
        Var* field_ptr = static_cast<const void*>(head_ptr + get_value_offset(field));
        type_descriptor.inspect_value_impl(field_ptr, target_ptr);
    }

    void inject_value_vptr(const type_descriptor& type_descriptor, const field& field, const void* source_ptr, reflectable& target)
    {
        Var* target_ptr = &target;
        Var* head_ptr = static_cast<char*>(static_cast<void*>(target_ptr));
        Var* field_ptr = static_cast<void*>(head_ptr + get_value_offset(field));
        type_descriptor.inject_value_impl(source_ptr, field_ptr);
    }

    void read_value_vptr(const type_descriptor& type_descriptor, const symbol& source_symbol, void* target_ptr)
    {
        type_descriptor.read_value_impl(source_symbol, target_ptr);
    }

    void write_value_vptr(const type_descriptor& type_descriptor, const void* source_ptr, symbol& target_symbol)
    {
        type_descriptor.write_value_impl(source_ptr, target_symbol);
    }

    void read_value(const symbol& source_symbol, reflectable& target_reflectable)
    {
        Val& type_descriptor = get_type_descriptor<reflectable>();
        read_value_vptr(*type_descriptor, source_symbol, static_cast<void*>(&target_reflectable));
    }

    void write_value(const reflectable& source_reflectable, symbol& target_symbol)
    {
        Val& type = get_type(source_reflectable);
        Val& type_index = get_type_index(*type);
        Val& type_descriptor = get_type_descriptor(type_index);
        write_value_vptr(*type_descriptor, static_cast<const void*>(&source_reflectable), target_symbol);
    }

    /* reflectable_descriptor */

    void reflectable_descriptor::inspect_value_impl(const void*, void*) const
    {
        throw std::runtime_error("reflectable_descriptor::inspect_value_impl not implemented.");
    }

    void reflectable_descriptor::inject_value_impl(const void*, void*) const
    {
        throw std::runtime_error("reflectable_descriptor::inject_value_impl not implemented.");
    }

    static void read_value_internal(const std::shared_ptr<type_t>& type, const symbol::right_type& symbol_tree, reflectable& reflectable)
    {
        // read sub-type values
        if (Val* base_type_index = get_base_type_index_opt(*type).get())
        {
            Val& base_type = get_type(*base_type_index);
            read_value_internal(base_type, symbol_tree, reflectable);
        }

        // read current type values
        Val& field_vector = get_field_vector(*type);
        Var symbol_tree_iter = std::begin(symbol_tree);
        Val symbol_tree_end = std::end(symbol_tree);
        for (Val& field_kvp : field_vector)
        {
            if (symbol_tree_iter != symbol_tree_end)
            {
                Val& field = field_kvp.second;
                Val& field_symbol = *symbol_tree_iter;
                Val& field_type_index = get_type_index(*field);
                Val& field_type_descriptor = get_type_descriptor(field_type_index);
                Val& field_ptr = static_cast<char*>(static_cast<void*>(&reflectable)) + get_value_offset(*field);
                read_value_vptr(*field_type_descriptor, field_symbol, field_ptr);
                ++symbol_tree_iter;
            }
        }
    }

    void write_value_internal(const std::shared_ptr<type_t>& type, const reflectable& reflectable, symbol::right_type& symbol_tree)
    {
        // write sub-type values
        if (Val* base_type_index = get_base_type_index_opt(*type).get())
        {
            Val& base_type = get_type(*base_type_index);
            write_value_internal(base_type, reflectable, symbol_tree);
        }

        // write current type values
        Val& field_vector = get_field_vector(*type);
        for (Val& field_kvp : field_vector)
        {
            symbol field_symbol_mvb{};
            Val& field = field_kvp.second;
            Val& field_type_index = get_type_index(*field);
            Val& field_type_descriptor = get_type_descriptor(field_type_index);
            Val& field_ptr = static_cast<const char*>(static_cast<const void*>(&reflectable)) + get_value_offset(*field);
            write_value_vptr(*field_type_descriptor, field_ptr, field_symbol_mvb);
            symbol_tree.emplace_back(std::move(field_symbol_mvb));
        }
    }

    void reflectable_descriptor::write_value_impl(const void* source_ptr, symbol& target_symbol) const
    {
        // ensure target is symbol tree
        if (!is_symbol_tree(target_symbol)) target_symbol = symbol_tree({});

        // get type to write
        Var& symbol_tree = get_symbol_tree(target_symbol);
        Val* reflectable_ptr = static_cast<const reflectable*>(source_ptr);
        Val& reflectable = *reflectable_ptr;
        Val& type = get_type(reflectable);

        // write values
        write_value_internal(type, reflectable, symbol_tree);
    }

    void reflectable_descriptor::read_value_impl(const symbol& source_symbol, void* target_ptr) const
    {
        Var* reflectable_ptr = static_cast<reflectable*>(target_ptr);
        match2(source_symbol,
        [&](Val& symbol_tree)
        {
            Var& reflectable = *reflectable_ptr;
            Val& type = get_type(reflectable);
            read_value_internal(type, symbol_tree, reflectable);
        },
        [](Val&) { throw std::invalid_argument("Expected symbol tree."); });
    }
}
