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
    class parse : public either<T, std::string>
    {
    public:

        CONSTRAINT(parse);
        template<typename A>
        using reify = parse<A>;
        using either<T, std::string>::either;

        bool is_success() const { return is_right(); }
        bool is_failure() const { return is_left(); }
        const T& get_success() const { return get_right(); }
        const std::string& get_failure() const { return get_left(); }

        const char* get_right_name() const override { return "parse_success"; }
        const char* get_left_name() const override { return "parse_failure"; }
    };

    // Make a successful parse result.
    template<typename T>
    parse<T> parse_success(const T& t) { return parse<T>(t, false); }

    // Make a failed parse result.
    template<typename T>
    parse<T> parse_failure(const std::string& error_str) { return parse<T>(error_str, false, false); }

    // Make a successful parse result.
    template<typename T>
    parse<T> parse_success(T&& value) { return parse<T>(value, false); }

    // Make a failed parse result.
    template<typename T>
    parse<T> parse_failure(std::string&& error_str) { return parse<T>(error_str, false, false); }

    // Try to run a parse, rewinding the parsing iterator upon failure.
    template<typename T, typename P>
    parse<T> try_parse(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end, P parser)
    {
        VAL iter_copy = iter;
        VAL& parse = parser(iter, end);
        if (!parse) iter = iter_copy;
        return parse;
    }

    // Skip zero or more whitespace chars.
    void skip_whitespace_many(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end);

    // Skip a char.
    parse<unit> skip_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end);

    // Skip the given char.
    parse<unit> skip_given_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end, char chr);

    // Parse a char.
    parse<char> parse_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end);

    // Parse a string until the given char is reached.
    parse<std::string> parse_until_given_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end, char chr);

    // Parse a string until any of the given chars are reached.
    parse<std::string> parse_until_any_given_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end, std::string chars);

    // Parse a symbol value from an xml buffer.
    parse<symbol> parse_symbol_from_xml_buffer(char* buffer);

    // Parse a symbol value from a std::istream.
    parse<symbol> parse_symbol_from_stream(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end);

    // Parse a symbol value from a std::string.
    parse<symbol> parse_symbol(const std::string& str);

    // Read a value of type T from a std::string (where T is a smart ptr where applicable).
    template<typename T>
    void read_value(const std::string& str, T& value)
    {
        match2(parse_symbol(str),
        [&](VAL& symbol)
        {
            VAL& type_descriptor = get_type_descriptor<T>();
            read_value(type_descriptor, symbol, &value);
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
        read_value(str, value);
        return value;
    }
}

#endif
