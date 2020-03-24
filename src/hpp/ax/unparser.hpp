#ifndef AX_UNPARSER_HPP
#define AX_UNPARSER_HPP

#include <string>

#include "symbol.hpp"
#include "type_descriptor.hpp"

namespace ax
{
    // Unparse an ax::symbol to a std::string.
    std::string unparse_symbol(const ax::symbol& symbol);

    // Write a value of type T to a std::string (where T is a smart ptr where applicable).
    template<typename T>
    void write_value(const T& value, std::string& str)
    {
        ax::symbol symbol;
        VAL& type_index = std::type_index(typeid(value));
        VAL& type_descriptor = get_type_descriptor(type_index);
        type_descriptor->write_value(&value, symbol);
        str = unparse_symbol(symbol);
    }

    // Convert a value of type T to a std::string (where T is a smart ptr where applicable).
    template<typename T>
    std::string stringize(const T& value)
    {
        std::string str;
        write_value<T>(value, str);
        return str;
    }
}

#endif
