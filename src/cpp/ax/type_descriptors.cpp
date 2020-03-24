#include <cstdlib>
#include <string>
#include <vector>

#include "ax/type_descriptors.hpp"

#include "ax/string.hpp"
#include "ax/record.hpp"
#include "ax/record4.hpp"
#include "ax/record5.hpp"
#include "ax/choice.hpp"
#include "ax/choice4.hpp"
#include "ax/choice5.hpp"
#include "ax/functional.hpp"
#include "ax/name.hpp"
#include "ax/address.hpp"

namespace ax
{
    /* bool_descriptor */

    void bool_descriptor::inspect_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<bool>(source_ptr, target_ptr);
    }

    void bool_descriptor::inject_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<bool>(source_ptr, target_ptr);
    }

    void bool_descriptor::read_value(const ax::symbol& source_symbol, void* target_ptr) const
    {
        VAR* bool_ptr = static_cast<bool*>(target_ptr);
        source_symbol.match(
            [&](VAL& atom) { *bool_ptr = atom != "false" && atom != "0"; },
            [](VAL&) { throw std::invalid_argument("Expected atom."); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); });
    }

    void bool_descriptor::write_value(const void* source_ptr, ax::symbol& target_symbol) const
    {
        VAL* bool_ptr = static_cast<const bool*>(source_ptr);
        target_symbol = ax::atom(*bool_ptr ? "true" : "false");
    }

    /* int_descriptor */

    void int_descriptor::inspect_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<int>(source_ptr, target_ptr);
    }

    void int_descriptor::inject_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<int>(source_ptr, target_ptr);
    }

    void int_descriptor::read_value(const ax::symbol& source_symbol, void* target_ptr) const
    {
        VAR* int_ptr = static_cast<int*>(target_ptr);
        source_symbol.match(
            [&](VAL& atom) { *int_ptr = static_cast<int>(std::strtoll(atom.c_str(), nullptr, 10)); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); });
    }

    void int_descriptor::write_value(const void* source_ptr, ax::symbol& target_symbol) const
    {
        VAL* int_ptr = static_cast<const int*>(source_ptr);
        target_symbol = atom(std::to_string(*int_ptr));
    }

    /* int32_descriptor */

    void int32_descriptor::inspect_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<int32_t>(source_ptr, target_ptr);
    }

    void int32_descriptor::inject_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<int32_t>(source_ptr, target_ptr);
    }

    void int32_descriptor::read_value(const ax::symbol& source_symbol, void* target_ptr) const
    {
        VAR* int32_ptr = static_cast<int32_t*>(target_ptr);
        source_symbol.match(
            [&](VAL& atom) { *int32_ptr = static_cast<int32_t>(std::strtoll(atom.c_str(), nullptr, 10)); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); });
    }

    void int32_descriptor::write_value(const void* source_ptr, ax::symbol& target_symbol) const
    {
        VAL* int32_ptr = static_cast<const int32_t*>(source_ptr);
        target_symbol = ax::atom(std::to_string(*int32_ptr));
    }

    /* int64_descriptor */

    void int64_descriptor::inspect_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<int64_t>(source_ptr, target_ptr);
    }

    void int64_descriptor::inject_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<int64_t>(source_ptr, target_ptr);
    }

    void int64_descriptor::read_value(const ax::symbol& source_symbol, void* target_ptr) const
    {
        VAR* int64_ptr = static_cast<int64_t*>(target_ptr);
        source_symbol.match(
            [&](VAL& atom) { *int64_ptr = static_cast<int64_t>(std::strtoll(atom.c_str(), nullptr, 10)); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); });
    }

    void int64_descriptor::write_value(const void* source_ptr, ax::symbol& target_symbol) const
    {
        VAL* int64_ptr = static_cast<const int64_t*>(source_ptr);
        target_symbol = ax::atom(std::to_string(*int64_ptr));
    }

    /* float_descriptor */

    void float_descriptor::inspect_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<float>(source_ptr, target_ptr);
    }

    void float_descriptor::inject_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<float>(source_ptr, target_ptr);
    }

    void float_descriptor::read_value(const ax::symbol& source_symbol, void* target_ptr) const
    {
        VAR* float_ptr = static_cast<float*>(target_ptr);
        source_symbol.match(
            [&](VAL& atom) { *float_ptr = static_cast<float>(std::strtod(atom.c_str(), nullptr)); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); });
    }

    void float_descriptor::write_value(const void* source_ptr, ax::symbol& target_symbol) const
    {
        VAL* float_ptr = static_cast<const float*>(source_ptr);
        target_symbol = ax::atom(std::to_string(*float_ptr));
    }

    /* double_descriptor */

    void double_descriptor::inspect_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<double>(source_ptr, target_ptr);
    }

    void double_descriptor::inject_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<double>(source_ptr, target_ptr);
    }

    void double_descriptor::read_value(const ax::symbol& source_symbol, void* target_ptr) const
    {
        VAR* double_ptr = static_cast<double*>(target_ptr);
        source_symbol.match(
            [&](VAL& atom) { *double_ptr = std::strtod(atom.c_str(), nullptr); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); });
    }

    void double_descriptor::write_value(const void* source_ptr, ax::symbol& target_symbol) const
    {
        VAL* double_ptr = static_cast<const double*>(source_ptr);
        target_symbol = ax::atom(std::to_string(*double_ptr));
    }

    /* size_descriptor */

    void size_descriptor::inspect_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<std::size_t>(source_ptr, target_ptr);
    }

    void size_descriptor::inject_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<std::size_t>(source_ptr, target_ptr);
    }

    void size_descriptor::read_value(const ax::symbol& source_symbol, void* target_ptr) const
    {
        VAR* size_ptr = static_cast<size_t*>(target_ptr);
        source_symbol.match(
            [&](VAL& atom) { *size_ptr = static_cast<size_t>(std::strtoll(atom.c_str(), nullptr, 10)); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); });
    }

    void size_descriptor::write_value(const void* source_ptr, ax::symbol& target_symbol) const
    {
        VAL* size_ptr = static_cast<const std::size_t*>(source_ptr);
        target_symbol = atom(std::to_string(*size_ptr));
    }

    /* string_descriptor */

    void string_descriptor::inspect_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<std::string>(source_ptr, target_ptr);
    }

    void string_descriptor::inject_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<std::string>(source_ptr, target_ptr);
    }

    void string_descriptor::read_value(const ax::symbol& source_symbol, void* target_ptr) const
    {
        VAR* string_ptr = static_cast<std::string*>(target_ptr);
        source_symbol.match(
            [&](VAL& atom) { *string_ptr = atom; },
            [](VAL&) { throw std::invalid_argument("Expected atom."); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); });
    }

    void string_descriptor::write_value(const void* source_ptr, ax::symbol& target_symbol) const
    {
        VAL* string_ptr = static_cast<const std::string*>(source_ptr);
        target_symbol = atom(*string_ptr);
    }

    /* name_descriptor */

    void name_descriptor::inspect_value(const void* source_ptr, void* target_ptr) const
    {
        assign_value<ax::name>(source_ptr, target_ptr);
    }

    void name_descriptor::inject_value(const void* source_ptr, void* target_ptr) const
    {
        assign_value<ax::name>(source_ptr, target_ptr);
    }

    void name_descriptor::read_value(const ax::symbol& source_symbol, void* target_ptr) const
    {
        VAR* name_ptr = static_cast<ax::name*>(target_ptr);
        source_symbol.match(
            [&](VAL& atom) { *name_ptr = atom; },
            [](VAL&) { throw std::invalid_argument("Expected atom."); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); });
    }

    void name_descriptor::write_value(const void* source_ptr, ax::symbol& target_symbol) const
    {
        VAL* name_ptr = static_cast<const ax::name*>(source_ptr);
        target_symbol = atom(name_ptr->to_string());
    }

    /* address_descriptor */

    void address_descriptor::inspect_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<ax::address>(source_ptr, target_ptr);
    }

    void address_descriptor::inject_value(const void* source_ptr, void* target_ptr) const
    {
        ax::assign_value<ax::address>(source_ptr, target_ptr);
    }

    void address_descriptor::read_value(const ax::symbol& source_symbol, void* target_ptr) const
    {
        VAR* address_ptr = static_cast<ax::address*>(target_ptr);
        source_symbol.match(
            [&](VAL& atom) { *address_ptr = ax::address(atom); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); },
            [](VAL&) { throw std::invalid_argument("Expected atom."); });
    }

    void address_descriptor::write_value(const void* source_ptr, ax::symbol& target_symbol) const
    {
        VAL* address_ptr = static_cast<const ax::address*>(source_ptr);
        VAL& address_names = address_ptr->get_names();
        VAL& address_strs = ax::map<std::string>(address_names, [](const ax::name& name) { return name.to_string(); });
        target_symbol = ax::atom(ax::join_strings(address_strs, '/'));
    }

    /* global functions */

    void register_common_type_descriptors()
    {
        // primitive type descriptors
        ax::register_type_descriptor<bool>(std::make_shared<ax::bool_descriptor>());
        ax::register_type_descriptor<int>(std::make_shared<ax::int_descriptor>());
        ax::register_type_descriptor<int32_t>(std::make_shared<ax::int32_descriptor>());
        ax::register_type_descriptor<int64_t>(std::make_shared<ax::int64_descriptor>());
        ax::register_type_descriptor<float>(std::make_shared<ax::float_descriptor>());
        ax::register_type_descriptor<double>(std::make_shared<ax::double_descriptor>());
        ax::register_type_descriptor<std::size_t>(std::make_shared<ax::size_descriptor>());
        ax::register_type_descriptor<std::string>(std::make_shared<ax::string_descriptor>());

        // primitive ax type descriptors
        ax::register_type_descriptor<ax::name>(std::make_shared<ax::name_descriptor>());
        ax::register_type_descriptor<ax::address>(std::make_shared<ax::address_descriptor>());

        // std::shared_ptr type descriptors
        ax::register_type_descriptor<std::shared_ptr<int>>(std::make_shared<ax::shared_ptr_descriptor<int>>());
        ax::register_type_descriptor<std::shared_ptr<std::string>>(std::make_shared<ax::shared_ptr_descriptor<std::string>>());
        ax::register_type_descriptor<std::shared_ptr<ax::reflectable>>(std::make_shared<ax::shared_ptr_descriptor<ax::reflectable>>());

        // std::vector type descriptors
        ax::register_type_descriptor<std::vector<bool>>(std::make_shared<ax::std_vector_descriptor<bool>>());
        ax::register_type_descriptor<std::vector<char>>(std::make_shared<ax::std_vector_descriptor<char>>());
        ax::register_type_descriptor<std::vector<int>>(std::make_shared<ax::std_vector_descriptor<int>>());
        ax::register_type_descriptor<std::vector<int32_t>>(std::make_shared<ax::std_vector_descriptor<int32_t>>());
        ax::register_type_descriptor<std::vector<int64_t>>(std::make_shared<ax::std_vector_descriptor<int64_t>>());
        ax::register_type_descriptor<std::vector<float>>(std::make_shared<ax::std_vector_descriptor<float>>());
        ax::register_type_descriptor<std::vector<double>>(std::make_shared<ax::std_vector_descriptor<double>>());
        ax::register_type_descriptor<std::vector<std::size_t>>(std::make_shared<ax::std_vector_descriptor<std::size_t>>());
        ax::register_type_descriptor<std::vector<std::string>>(std::make_shared<ax::std_vector_descriptor<std::string>>());

        // std::unordered_set type descriptors
        ax::register_type_descriptor<std::unordered_set<bool>>(std::make_shared<ax::std_unordered_set_descriptor<bool>>());
        ax::register_type_descriptor<std::unordered_set<char>>(std::make_shared<ax::std_unordered_set_descriptor<char>>());
        ax::register_type_descriptor<std::unordered_set<int>>(std::make_shared<ax::std_unordered_set_descriptor<int>>());
        ax::register_type_descriptor<std::unordered_set<int32_t>>(std::make_shared<ax::std_unordered_set_descriptor<int32_t>>());
        ax::register_type_descriptor<std::unordered_set<int64_t>>(std::make_shared<ax::std_unordered_set_descriptor<int64_t>>());
        ax::register_type_descriptor<std::unordered_set<float>>(std::make_shared<ax::std_unordered_set_descriptor<float>>());
        ax::register_type_descriptor<std::unordered_set<double>>(std::make_shared<ax::std_unordered_set_descriptor<double>>());
        ax::register_type_descriptor<std::unordered_set<std::size_t>>(std::make_shared<ax::std_unordered_set_descriptor<std::size_t>>());
        ax::register_type_descriptor<std::unordered_set<std::string>>(std::make_shared<ax::std_unordered_set_descriptor<std::string>>());

        // std::unordered_map type descriptors
        ax::register_type_descriptor<std::unordered_map<std::string, bool>>(std::make_shared<ax::std_unordered_map_descriptor<std::string, bool>>());
        ax::register_type_descriptor<std::unordered_map<std::string, char>>(std::make_shared<ax::std_unordered_map_descriptor<std::string, char>>());
        ax::register_type_descriptor<std::unordered_map<std::string, int>>(std::make_shared<ax::std_unordered_map_descriptor<std::string, int>>());
        ax::register_type_descriptor<std::unordered_map<std::string, int32_t>>(std::make_shared<ax::std_unordered_map_descriptor<std::string, int32_t>>());
        ax::register_type_descriptor<std::unordered_map<std::string, int64_t>>(std::make_shared<ax::std_unordered_map_descriptor<std::string, int64_t>>());
        ax::register_type_descriptor<std::unordered_map<std::string, float>>(std::make_shared<ax::std_unordered_map_descriptor<std::string, float>>());
        ax::register_type_descriptor<std::unordered_map<std::string, double>>(std::make_shared<ax::std_unordered_map_descriptor<std::string, double>>());
        ax::register_type_descriptor<std::unordered_map<std::string, std::size_t>>(std::make_shared<ax::std_unordered_map_descriptor<std::string, std::size_t>>());
        ax::register_type_descriptor<std::unordered_map<std::string, std::string>>(std::make_shared<ax::std_unordered_map_descriptor<std::string, std::string>>());

        // ax::vector type descriptors
        // UNSUPPORTED - ax::register_type_descriptor<ax::vector<bool>>(std::make_shared<ax::ax_vector_descriptor<bool>>());
        ax::register_type_descriptor<ax::vector<char>>(std::make_shared<ax::ax_vector_descriptor<char>>());
        ax::register_type_descriptor<ax::vector<int>>(std::make_shared<ax::ax_vector_descriptor<int>>());
        ax::register_type_descriptor<ax::vector<int32_t>>(std::make_shared<ax::ax_vector_descriptor<int32_t>>());
        ax::register_type_descriptor<ax::vector<int64_t>>(std::make_shared<ax::ax_vector_descriptor<int64_t>>());
        ax::register_type_descriptor<ax::vector<float>>(std::make_shared<ax::ax_vector_descriptor<float>>());
        ax::register_type_descriptor<ax::vector<double>>(std::make_shared<ax::ax_vector_descriptor<double>>());
        ax::register_type_descriptor<ax::vector<std::size_t>>(std::make_shared<ax::ax_vector_descriptor<std::size_t>>());
        ax::register_type_descriptor<ax::vector<std::string>>(std::make_shared<ax::ax_vector_descriptor<std::string>>());

        // ax::pair type descriptors
        ax::register_type_descriptor<ax::pair<bool, bool>>(std::make_shared<ax::ax_pair_descriptor<ax::pair<bool, bool>>>());
        ax::register_type_descriptor<ax::pair<char, char>>(std::make_shared<ax::ax_pair_descriptor<ax::pair<char, char>>>());
        ax::register_type_descriptor<ax::pair<int, int>>(std::make_shared<ax::ax_pair_descriptor<ax::pair<int, int>>>());
        ax::register_type_descriptor<ax::pair<int32_t, int32_t>>(std::make_shared<ax::ax_pair_descriptor<ax::pair<int32_t, int32_t>>>());
        ax::register_type_descriptor<ax::pair<int64_t, int64_t>>(std::make_shared<ax::ax_pair_descriptor<ax::pair<int64_t, int64_t>>>());
        ax::register_type_descriptor<ax::pair<float, float>>(std::make_shared<ax::ax_pair_descriptor<ax::pair<float, float>>>());
        ax::register_type_descriptor<ax::pair<double, double>>(std::make_shared<ax::ax_pair_descriptor<ax::pair<double, double>>>());
        ax::register_type_descriptor<ax::pair<std::size_t, std::size_t>>(std::make_shared<ax::ax_pair_descriptor<ax::pair<std::size_t, std::size_t>>>());
        ax::register_type_descriptor<ax::pair<std::string, std::string>>(std::make_shared<ax::ax_pair_descriptor<ax::pair<std::string, std::string>>>());

        // record type descriptor (ints only)
        ax::register_type_descriptor<ax::record<int, int, int>>(std::make_shared<ax::record_descriptor<ax::record<int, int, int>>>());

        // record4 type descriptor (ints only)
        ax::register_type_descriptor<ax::record4<int, int, int, int>>(std::make_shared<ax::record4_descriptor<ax::record4<int, int, int, int>>>());

        // record5 type descriptor (ints only)
        ax::register_type_descriptor<ax::record5<int, int, int, int, int>>(std::make_shared<ax::record5_descriptor<ax::record5<int, int, int, int, int>>>());

        // option type descriptor
        ax::register_type_descriptor<ax::option<bool>>(std::make_shared<ax::option_descriptor<bool>>());
        ax::register_type_descriptor<ax::option<char>>(std::make_shared<ax::option_descriptor<char>>());
        ax::register_type_descriptor<ax::option<int>>(std::make_shared<ax::option_descriptor<int>>());
        ax::register_type_descriptor<ax::option<int32_t>>(std::make_shared<ax::option_descriptor<int32_t>>());
        ax::register_type_descriptor<ax::option<int64_t>>(std::make_shared<ax::option_descriptor<int64_t>>());
        ax::register_type_descriptor<ax::option<float>>(std::make_shared<ax::option_descriptor<float>>());
        ax::register_type_descriptor<ax::option<double>>(std::make_shared<ax::option_descriptor<double>>());
        ax::register_type_descriptor<ax::option<std::size_t>>(std::make_shared<ax::option_descriptor<std::size_t>>());
        ax::register_type_descriptor<ax::option<std::string>>(std::make_shared<ax::option_descriptor<std::string>>());

        // either type descriptors (with left as 'error' string)
        ax::register_type_descriptor<ax::either<bool, std::string>>(std::make_shared<ax::either_descriptor<ax::either<bool, std::string>>>());
        ax::register_type_descriptor<ax::either<char, std::string>>(std::make_shared<ax::either_descriptor<ax::either<char, std::string>>>());
        ax::register_type_descriptor<ax::either<int, std::string>>(std::make_shared<ax::either_descriptor<ax::either<int, std::string>>>());
        ax::register_type_descriptor<ax::either<int32_t, std::string>>(std::make_shared<ax::either_descriptor<ax::either<int32_t, std::string>>>());
        ax::register_type_descriptor<ax::either<int64_t, std::string>>(std::make_shared<ax::either_descriptor<ax::either<int64_t, std::string>>>());
        ax::register_type_descriptor<ax::either<float, std::string>>(std::make_shared<ax::either_descriptor<ax::either<float, std::string>>>());
        ax::register_type_descriptor<ax::either<double, std::string>>(std::make_shared<ax::either_descriptor<ax::either<double, std::string>>>());
        ax::register_type_descriptor<ax::either<std::size_t, std::string>>(std::make_shared<ax::either_descriptor<ax::either<std::size_t, std::string>>>());
        ax::register_type_descriptor<ax::either<std::string, std::string>>(std::make_shared<ax::either_descriptor<ax::either<std::string, std::string>>>());

        // either type descriptor (ints only)
        ax::register_type_descriptor<ax::either<int, int>>(std::make_shared<ax::either_descriptor<ax::either<int, int>>>());

        // choice type descriptor (ints only)
        ax::register_type_descriptor<ax::choice<int, int, int>>(std::make_shared<ax::choice_descriptor<ax::choice<int, int, int>>>());

        // choice4 type descriptor (ints only)
        ax::register_type_descriptor<ax::choice4<int, int, int, int>>(std::make_shared<ax::choice4_descriptor<ax::choice4<int, int, int, int>>>());

        // choice5 type descriptor (ints only)
        ax::register_type_descriptor<ax::choice5<int, int, int, int, int>>(std::make_shared<ax::choice5_descriptor<ax::choice5<int, int, int, int, int>>>());
    }
}
