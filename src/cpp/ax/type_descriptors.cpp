#include <cstdlib>
#include <string>
#include <vector>

#include "../../dpp/ax/type_descriptors.hpp"

#include "../../dpp/ax/choice.hpp"
#include "../../dpp/ax/string.hpp"
#include "../../dpp/ax/functional.hpp"
#include "../../dpp/ax/name.hpp"
#include "../../dpp/ax/address.hpp"

namespace ax
{
    /* bool_descriptor */

    void bool_descriptor::inspect_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<bool>(source_ptr, target_ptr);
    }

    void bool_descriptor::inject_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<bool>(source_ptr, target_ptr);
    }

    void bool_descriptor::read_value_impl(const symbol& source_symbol, void* target_ptr) const
    {
        var* bool_ptr = static_cast<bool*>(target_ptr);
        match2(source_symbol,
        [](val&) { throw std::invalid_argument("Expected symbol leaf."); },
        [&](val& symbol_leaf) { *bool_ptr = symbol_leaf != "false" && symbol_leaf != "0"; });
    }

    void bool_descriptor::write_value_impl(const void* source_ptr, symbol& target_symbol) const
    {
        val* bool_ptr = static_cast<const bool*>(source_ptr);
        target_symbol = symbol_leaf(*bool_ptr ? "true" : "false");
    }

    /* int_descriptor */

    void int_descriptor::inspect_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<int>(source_ptr, target_ptr);
    }

    void int_descriptor::inject_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<int>(source_ptr, target_ptr);
    }

    void int_descriptor::read_value_impl(const symbol& source_symbol, void* target_ptr) const
    {
        var* int_ptr = static_cast<int*>(target_ptr);
        match2(source_symbol,
        [](val&) { throw std::invalid_argument("Expected symbol leaf."); },
        [&](val& symbol_leaf) { *int_ptr = static_cast<int>(strtoll(symbol_leaf.c_str(), nullptr, 10)); });
    }

    void int_descriptor::write_value_impl(const void* source_ptr, symbol& target_symbol) const
    {
        val* int_ptr = static_cast<const int*>(source_ptr);
        target_symbol = symbol_leaf(to_string(*int_ptr));
    }

    /* int32_descriptor */

    void int32_descriptor::inspect_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<int32_t>(source_ptr, target_ptr);
    }

    void int32_descriptor::inject_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<int32_t>(source_ptr, target_ptr);
    }

    void int32_descriptor::read_value_impl(const symbol& source_symbol, void* target_ptr) const
    {
        var* int32_ptr = static_cast<int32_t*>(target_ptr);
        match2(source_symbol,
        [](val&) { throw std::invalid_argument("Expected symbol leaf."); },
        [&](val& symbol_leaf) { *int32_ptr = static_cast<int32_t>(strtoll(symbol_leaf.c_str(), nullptr, 10)); });
    }

    void int32_descriptor::write_value_impl(const void* source_ptr, symbol& target_symbol) const
    {
        val* int32_ptr = static_cast<const int32_t*>(source_ptr);
        target_symbol = symbol_leaf(to_string(*int32_ptr));
    }

    /* int64_descriptor */

    void int64_descriptor::inspect_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<int64_t>(source_ptr, target_ptr);
    }

    void int64_descriptor::inject_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<int64_t>(source_ptr, target_ptr);
    }

    void int64_descriptor::read_value_impl(const symbol& source_symbol, void* target_ptr) const
    {
        var* int64_ptr = static_cast<int64_t*>(target_ptr);
        match2(source_symbol,
        [](val&) { throw std::invalid_argument("Expected symbol leaf."); },
        [&](val& symbol_leaf) { *int64_ptr = static_cast<int64_t>(strtoll(symbol_leaf.c_str(), nullptr, 10)); });
    }

    void int64_descriptor::write_value_impl(const void* source_ptr, symbol& target_symbol) const
    {
        val* int64_ptr = static_cast<const int64_t*>(source_ptr);
        target_symbol = symbol_leaf(to_string(*int64_ptr));
    }

    /* float_descriptor */

    void float_descriptor::inspect_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<float>(source_ptr, target_ptr);
    }

    void float_descriptor::inject_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<float>(source_ptr, target_ptr);
    }

    void float_descriptor::read_value_impl(const symbol& source_symbol, void* target_ptr) const
    {
        var* float_ptr = static_cast<float*>(target_ptr);
        match2(source_symbol,
        [](val&) { throw std::invalid_argument("Expected symbol leaf."); },
        [&](val& symbol_leaf) { *float_ptr = static_cast<float>(strtod(symbol_leaf.c_str(), nullptr)); });
    }

    void float_descriptor::write_value_impl(const void* source_ptr, symbol& target_symbol) const
    {
        val* float_ptr = static_cast<const float*>(source_ptr);
        target_symbol = symbol_leaf(to_string(*float_ptr));
    }

    /* double_descriptor */

    void double_descriptor::inspect_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<double>(source_ptr, target_ptr);
    }

    void double_descriptor::inject_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<double>(source_ptr, target_ptr);
    }

    void double_descriptor::read_value_impl(const symbol& source_symbol, void* target_ptr) const
    {
        var* double_ptr = static_cast<double*>(target_ptr);
        match2(source_symbol,
        [](val&) { throw std::invalid_argument("Expected symbol leaf."); },
        [&](val& symbol_leaf) { *double_ptr = strtod(symbol_leaf.c_str(), nullptr); });
    }

    void double_descriptor::write_value_impl(const void* source_ptr, symbol& target_symbol) const
    {
        val* double_ptr = static_cast<const double*>(source_ptr);
        target_symbol = symbol_leaf(to_string(*double_ptr));
    }

    /* size_descriptor */

    void size_descriptor::inspect_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<std::size_t>(source_ptr, target_ptr);
    }

    void size_descriptor::inject_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<std::size_t>(source_ptr, target_ptr);
    }

    void size_descriptor::read_value_impl(const symbol& source_symbol, void* target_ptr) const
    {
        var* size_ptr = static_cast<size_t*>(target_ptr);
        match2(source_symbol,
        [](val&) { throw std::invalid_argument("Expected symbol leaf."); },
        [&](val& symbol_leaf) { *size_ptr = static_cast<size_t>(strtoll(symbol_leaf.c_str(), nullptr, 10)); });
    }

    void size_descriptor::write_value_impl(const void* source_ptr, symbol& target_symbol) const
    {
        val* size_ptr = static_cast<const size_t*>(source_ptr);
        target_symbol = symbol_leaf(to_string(*size_ptr));
    }

    /* string_descriptor */

    void string_descriptor::inspect_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<std::string>(source_ptr, target_ptr);
    }

    void string_descriptor::inject_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<std::string>(source_ptr, target_ptr);
    }

    void string_descriptor::read_value_impl(const symbol& source_symbol, void* target_ptr) const
    {
        var* string_ptr = static_cast<std::string*>(target_ptr);
        match2(source_symbol,
        [](val&) { throw std::invalid_argument("Expected symbol leaf."); },
        [&](val& symbol_leaf) { *string_ptr = symbol_leaf; });
    }

    void string_descriptor::write_value_impl(const void* source_ptr, symbol& target_symbol) const
    {
        val* string_ptr = static_cast<const std::string*>(source_ptr);
        target_symbol = symbol_leaf(*string_ptr);
    }

    /* name_descriptor */

    void name_descriptor::inspect_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<name_t>(source_ptr, target_ptr);
    }

    void name_descriptor::inject_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<name_t>(source_ptr, target_ptr);
    }

    void name_descriptor::read_value_impl(const symbol& source_symbol, void* target_ptr) const
    {
        var* name_ptr = static_cast<name_t*>(target_ptr);
        match2(source_symbol,
        [](val&) { throw std::invalid_argument("Expected symbol leaf."); },
        [&](val& symbol_leaf) { *name_ptr = symbol_leaf; });
    }

    void name_descriptor::write_value_impl(const void* source_ptr, symbol& target_symbol) const
    {
        val* name_ptr = static_cast<const name_t*>(source_ptr);
        target_symbol = symbol_leaf(get_name_str(*name_ptr));
    }

    /* address_descriptor */

    void address_descriptor::inspect_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<address>(source_ptr, target_ptr);
    }

    void address_descriptor::inject_value_impl(const void* source_ptr, void* target_ptr) const
    {
        assign_value_vptr<address>(source_ptr, target_ptr);
    }

    void address_descriptor::read_value_impl(const symbol& source_symbol, void* target_ptr) const
    {
        var* address_ptr = static_cast<address*>(target_ptr);
        match2(source_symbol,
        [](val&) { throw std::invalid_argument("Expected symbol leaf."); },
        [&](val& symbol_leaf) { *address_ptr = address(symbol_leaf); });
    }

    void address_descriptor::write_value_impl(const void* source_ptr, symbol& target_symbol) const
    {
        val* address_ptr = static_cast<const address*>(source_ptr);
        val& address_names = get_names(*address_ptr);
        val& address_strs = map<std::string>(address_names, get_name_str);
        target_symbol = symbol_leaf(join_strings(address_strs, '/'));
    }

    /* global functions */

    void register_common_type_descriptors()
    {
        // primitive type descriptors
        register_type_descriptor<bool>(std::make_shared<bool_descriptor>());
        register_type_descriptor<int>(std::make_shared<int_descriptor>());
        register_type_descriptor<int32_t>(std::make_shared<int32_descriptor>());
        register_type_descriptor<int64_t>(std::make_shared<int64_descriptor>());
        register_type_descriptor<float>(std::make_shared<float_descriptor>());
        register_type_descriptor<double>(std::make_shared<double_descriptor>());
        register_type_descriptor<std::size_t>(std::make_shared<size_descriptor>());
        register_type_descriptor<std::string>(std::make_shared<string_descriptor>());

        // primitive ax type descriptors
        register_type_descriptor<name_t>(std::make_shared<name_descriptor>());
        register_type_descriptor<address>(std::make_shared<address_descriptor>());

        // std::vector type descriptors
        register_type_descriptor<std::vector<bool>>(std::make_shared<vector_descriptor<bool>>());
        register_type_descriptor<std::vector<char>>(std::make_shared<vector_descriptor<char>>());
        register_type_descriptor<std::vector<int>>(std::make_shared<vector_descriptor<int>>());
        register_type_descriptor<std::vector<int32_t>>(std::make_shared<vector_descriptor<int32_t>>());
        register_type_descriptor<std::vector<int64_t>>(std::make_shared<vector_descriptor<int64_t>>());
        register_type_descriptor<std::vector<float>>(std::make_shared<vector_descriptor<float>>());
        register_type_descriptor<std::vector<double>>(std::make_shared<vector_descriptor<double>>());
        register_type_descriptor<std::vector<std::size_t>>(std::make_shared<vector_descriptor<std::size_t>>());
        register_type_descriptor<std::vector<std::string>>(std::make_shared<vector_descriptor<std::string>>());

        // std::unique_ptr type descriptors
        register_type_descriptor<std::unique_ptr<bool>>(std::make_shared<unique_ptr_descriptor<bool>>());
        register_type_descriptor<std::unique_ptr<char>>(std::make_shared<unique_ptr_descriptor<char>>());
        register_type_descriptor<std::unique_ptr<int>>(std::make_shared<unique_ptr_descriptor<int>>());
        register_type_descriptor<std::unique_ptr<int32_t>>(std::make_shared<unique_ptr_descriptor<int32_t>>());
        register_type_descriptor<std::unique_ptr<int64_t>>(std::make_shared<unique_ptr_descriptor<int64_t>>());
        register_type_descriptor<std::unique_ptr<float>>(std::make_shared<unique_ptr_descriptor<float>>());
        register_type_descriptor<std::unique_ptr<double>>(std::make_shared<unique_ptr_descriptor<double>>());
        register_type_descriptor<std::unique_ptr<std::size_t>>(std::make_shared<unique_ptr_descriptor<std::size_t>>());
        register_type_descriptor<std::unique_ptr<std::string>>(std::make_shared<unique_ptr_descriptor<std::string>>());

        // std::shared_ptr type descriptors
        register_type_descriptor<std::shared_ptr<bool>>(std::make_shared<shared_ptr_descriptor<bool>>());
        register_type_descriptor<std::shared_ptr<char>>(std::make_shared<shared_ptr_descriptor<char>>());
        register_type_descriptor<std::shared_ptr<int>>(std::make_shared<shared_ptr_descriptor<int>>());
        register_type_descriptor<std::shared_ptr<int32_t>>(std::make_shared<shared_ptr_descriptor<int32_t>>());
        register_type_descriptor<std::shared_ptr<int64_t>>(std::make_shared<shared_ptr_descriptor<int64_t>>());
        register_type_descriptor<std::shared_ptr<float>>(std::make_shared<shared_ptr_descriptor<float>>());
        register_type_descriptor<std::shared_ptr<double>>(std::make_shared<shared_ptr_descriptor<double>>());
        register_type_descriptor<std::shared_ptr<std::size_t>>(std::make_shared<shared_ptr_descriptor<std::size_t>>());
        register_type_descriptor<std::shared_ptr<std::string>>(std::make_shared<shared_ptr_descriptor<std::string>>());

        // option type descriptors
        register_type_descriptor<std::unordered_set<bool>>(std::make_shared<unordered_set_descriptor<bool>>());
        register_type_descriptor<std::unordered_set<char>>(std::make_shared<unordered_set_descriptor<char>>());
        register_type_descriptor<std::unordered_set<int>>(std::make_shared<unordered_set_descriptor<int>>());
        register_type_descriptor<std::unordered_set<int32_t>>(std::make_shared<unordered_set_descriptor<int32_t>>());
        register_type_descriptor<std::unordered_set<int64_t>>(std::make_shared<unordered_set_descriptor<int64_t>>());
        register_type_descriptor<std::unordered_set<float>>(std::make_shared<unordered_set_descriptor<float>>());
        register_type_descriptor<std::unordered_set<double>>(std::make_shared<unordered_set_descriptor<double>>());
        register_type_descriptor<std::unordered_set<std::size_t>>(std::make_shared<unordered_set_descriptor<std::size_t>>());
        register_type_descriptor<std::unordered_set<std::string>>(std::make_shared<unordered_set_descriptor<std::string>>());

        // std::unordered_map type descriptors
        register_type_descriptor<std::unordered_map<std::string, bool>>(std::make_shared<unordered_map_descriptor<std::string, bool>>());
        register_type_descriptor<std::unordered_map<std::string, char>>(std::make_shared<unordered_map_descriptor<std::string, char>>());
        register_type_descriptor<std::unordered_map<std::string, int>>(std::make_shared<unordered_map_descriptor<std::string, int>>());
        register_type_descriptor<std::unordered_map<std::string, int32_t>>(std::make_shared<unordered_map_descriptor<std::string, int32_t>>());
        register_type_descriptor<std::unordered_map<std::string, int64_t>>(std::make_shared<unordered_map_descriptor<std::string, int64_t>>());
        register_type_descriptor<std::unordered_map<std::string, float>>(std::make_shared<unordered_map_descriptor<std::string, float>>());
        register_type_descriptor<std::unordered_map<std::string, double>>(std::make_shared<unordered_map_descriptor<std::string, double>>());
        register_type_descriptor<std::unordered_map<std::string, std::size_t>>(std::make_shared<unordered_map_descriptor<std::string, std::size_t>>());
        register_type_descriptor<std::unordered_map<std::string, std::string>>(std::make_shared<unordered_map_descriptor<std::string, std::string>>());

        // pair type descriptors
        register_type_descriptor<pair<bool, bool>>(std::make_shared<pair_descriptor<pair<bool, bool>>>());
        register_type_descriptor<pair<char, char>>(std::make_shared<pair_descriptor<pair<char, char>>>());
        register_type_descriptor<pair<int, int>>(std::make_shared<pair_descriptor<pair<int, int>>>());
        register_type_descriptor<pair<int32_t, int32_t>>(std::make_shared<pair_descriptor<pair<int32_t, int32_t>>>());
        register_type_descriptor<pair<int64_t, int64_t>>(std::make_shared<pair_descriptor<pair<int64_t, int64_t>>>());
        register_type_descriptor<pair<float, float>>(std::make_shared<pair_descriptor<pair<float, float>>>());
        register_type_descriptor<pair<double, double>>(std::make_shared<pair_descriptor<pair<double, double>>>());
        register_type_descriptor<pair<std::size_t, std::size_t>>(std::make_shared<pair_descriptor<pair<std::size_t, std::size_t>>>());
        register_type_descriptor<pair<std::string, std::string>>(std::make_shared<pair_descriptor<pair<std::string, std::string>>>());

        // record type descriptors
        register_type_descriptor<record<bool, bool, bool>>(std::make_shared<record_descriptor<record<bool, bool, bool>>>());
        register_type_descriptor<record<char, char, char>>(std::make_shared<record_descriptor<record<char, char, char>>>());
        register_type_descriptor<record<int, int, int>>(std::make_shared<record_descriptor<record<int, int, int>>>());
        register_type_descriptor<record<int32_t, int32_t, int32_t>>(std::make_shared<record_descriptor<record<int32_t, int32_t, int32_t>>>());
        register_type_descriptor<record<int64_t, int64_t, int64_t>>(std::make_shared<record_descriptor<record<int64_t, int64_t, int64_t>>>());
        register_type_descriptor<record<float, float, float>>(std::make_shared<record_descriptor<record<float, float, float>>>());
        register_type_descriptor<record<double, double, double>>(std::make_shared<record_descriptor<record<double, double, double>>>());
        register_type_descriptor<record<std::size_t, std::size_t, std::size_t>>(std::make_shared<record_descriptor<record<std::size_t, std::size_t, std::size_t>>>());
        register_type_descriptor<record<std::string, std::string, std::string>>(std::make_shared<record_descriptor<record<std::string, std::string, std::string>>>());

        // option type descriptors
        register_type_descriptor<option<bool>>(std::make_shared<option_descriptor<bool>>());
        register_type_descriptor<option<char>>(std::make_shared<option_descriptor<char>>());
        register_type_descriptor<option<int>>(std::make_shared<option_descriptor<int>>());
        register_type_descriptor<option<int32_t>>(std::make_shared<option_descriptor<int32_t>>());
        register_type_descriptor<option<int64_t>>(std::make_shared<option_descriptor<int64_t>>());
        register_type_descriptor<option<float>>(std::make_shared<option_descriptor<float>>());
        register_type_descriptor<option<double>>(std::make_shared<option_descriptor<double>>());
        register_type_descriptor<option<std::size_t>>(std::make_shared<option_descriptor<std::size_t>>());
        register_type_descriptor<option<std::string>>(std::make_shared<option_descriptor<std::string>>());

        // either type descriptors
        register_type_descriptor<either<bool, bool>>(std::make_shared<either_descriptor<either<bool, bool>>>());
        register_type_descriptor<either<char, char>>(std::make_shared<either_descriptor<either<char, char>>>());
        register_type_descriptor<either<int, int>>(std::make_shared<either_descriptor<either<int, int>>>());
        register_type_descriptor<either<int32_t, int32_t>>(std::make_shared<either_descriptor<either<int32_t, int32_t>>>());
        register_type_descriptor<either<int64_t, int64_t>>(std::make_shared<either_descriptor<either<int64_t, int64_t>>>());
        register_type_descriptor<either<float, float>>(std::make_shared<either_descriptor<either<float, float>>>());
        register_type_descriptor<either<double, double>>(std::make_shared<either_descriptor<either<double, double>>>());
        register_type_descriptor<either<std::size_t, std::size_t>>(std::make_shared<either_descriptor<either<std::size_t, std::size_t>>>());
        register_type_descriptor<either<std::string, std::string>>(std::make_shared<either_descriptor<either<std::string, std::string>>>());

        // either type descriptors (with left as error string)
        register_type_descriptor<either<std::string, bool>>(std::make_shared<either_descriptor<either<std::string, bool>>>());
        register_type_descriptor<either<std::string, char>>(std::make_shared<either_descriptor<either<std::string, char>>>());
        register_type_descriptor<either<std::string, int>>(std::make_shared<either_descriptor<either<std::string, int>>>());
        register_type_descriptor<either<std::string, int32_t>>(std::make_shared<either_descriptor<either<std::string, int32_t>>>());
        register_type_descriptor<either<std::string, int64_t>>(std::make_shared<either_descriptor<either<std::string, int64_t>>>());
        register_type_descriptor<either<std::string, float>>(std::make_shared<either_descriptor<either<std::string, float>>>());
        register_type_descriptor<either<std::string, double>>(std::make_shared<either_descriptor<either<std::string, double>>>());
        register_type_descriptor<either<std::string, std::size_t>>(std::make_shared<either_descriptor<either<std::string, std::size_t>>>());
        register_type_descriptor<either<std::string, std::string>>(std::make_shared<either_descriptor<either<std::string, std::string>>>());

        // choice type descriptors
        register_type_descriptor<choice<bool, bool, bool>>(std::make_shared<choice_descriptor<choice<bool, bool, bool>>>());
        register_type_descriptor<choice<char, char, char>>(std::make_shared<choice_descriptor<choice<char, char, char>>>());
        register_type_descriptor<choice<int, int, int>>(std::make_shared<choice_descriptor<choice<int, int, int>>>());
        register_type_descriptor<choice<int32_t, int32_t, int32_t>>(std::make_shared<choice_descriptor<choice<int32_t, int32_t, int32_t>>>());
        register_type_descriptor<choice<int64_t, int64_t, int64_t>>(std::make_shared<choice_descriptor<choice<int64_t, int64_t, int64_t>>>());
        register_type_descriptor<choice<float, float, float>>(std::make_shared<choice_descriptor<choice<float, float, float>>>());
        register_type_descriptor<choice<double, double, double>>(std::make_shared<choice_descriptor<choice<double, double, double>>>());
        register_type_descriptor<choice<std::size_t, std::size_t, std::size_t>>(std::make_shared<choice_descriptor<choice<std::size_t, std::size_t, std::size_t>>>());
        register_type_descriptor<choice<std::string, std::string, std::string>>(std::make_shared<choice_descriptor<choice<std::string, std::string, std::string>>>());
    }
}
