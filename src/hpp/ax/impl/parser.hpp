#ifndef IMPL_AX_PARSER_HPP
#define IMPL_AX_PARSER_HPP

#include <cstddef>
#include <string>
#include <iterator>

#include "prelude.hpp"
#include "either.hpp"
#include "symbol.hpp"
#include "type_descriptor.hpp"

namespace ax
{
    // The result of a parser.
    template<typename T>
    class parse final : public ax::either<T, std::string>
    {
    public:

        CONSTRAINT(parse);
        template<typename A> using reify = ax::parse<A>;
        using either<T, std::string>::either;

        bool is_success() const { return either<T, std::string>::is_right(); }
        bool is_failure() const { return either<T, std::string>::is_left(); }
        const T& get_success() const { return either<T, std::string>::get_right(); }
        const std::string& get_failure() const { return either<T, std::string>::get_left(); }

        const char* get_right_name() const override { return "parse_success"; }
        const char* get_left_name() const override { return "parse_failure"; }
    };

    // Make a successful parse result.
    template<typename T>
    ax::parse<T> parse_success(const T& t) { return ax::parse<T>(t, false); }

    // Make a failed parse result.
    template<typename T>
    ax::parse<T> parse_failure(const std::string& error_str) { return ax::parse<T>(error_str, false, false); }

    // Make a successful parse result.
    template<typename T>
    ax::parse<T> parse_success(T&& value) { return ax::parse<T>(value, false); }

    // Make a failed parse result.
    template<typename T>
    ax::parse<T> parse_failure(std::string&& error_str) { return ax::parse<T>(error_str, false, false); }

    // Try to run a parse, rewinding the parsing iterator upon failure.
    template<typename T, typename P>
    ax::parse<T> try_parse(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end, P parser)
    {
        VAL iter_copy = iter;
        VAL& parse = parser(iter, end);
        if (!parse) iter = iter_copy;
        return parse;
    }

    // Skip zero or more whitespace chars.
    void skip_whitespace_many(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end);

    // Skip a char.
    ax::parse<unit> skip_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end);

    // Skip the given char.
    ax::parse<unit> skip_given_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end, char chr);

    // Parse a char.
    ax::parse<char> parse_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end);

    // Parse a string until the given char is reached.
    ax::parse<std::string> parse_until_given_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end, char chr);

    // Parse a string until any of the given chars are reached.
    ax::parse<std::string> parse_until_any_given_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end, std::string chars);

    // Parse a symbol value from an xml buffer.
    ax::parse<symbol> parse_symbol_from_xml_buffer(char* buffer);

    // Parse a symbol value from a std::istream.
    ax::parse<symbol> parse_symbol_from_stream(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end);

    // Parse a symbol value from a std::string.
    ax::parse<symbol> parse_symbol(const std::string& str);

    // Read a value of type T from a std::string (where T is a smart ptr where applicable).
    template<typename T>
    void read_value(const std::string& str, T& value)
    {
        ax::parse_symbol(str).match(
        [&](VAL& symbol)
        {
            VAL& type_descriptor = ax::get_type_descriptor<T>();
            type_descriptor.read_value(symbol, &value);
        },
        [&](VAL& error)
        {
            throw std::runtime_error(
                "Could not parse value of type '"_s + typeid(T).name() +
                "' from string '" + str +
                "' due to '" + error +
                "'.");
        });
    }

    // Convert a std::string to a value of type T (where T is a smart ptr where applicable).
    template<typename T>
    T valueize(const std::string& str)
    {
        T value;
        ax::read_value(str, value);
        return value;
    }
}

#endif
