#ifndef IMPL_AX_PARSER_HPP
#define IMPL_AX_PARSER_HPP

#include <cstddef>
#include <string>
#include <vector>
#include <iterator>

#include "prelude.hpp"
#include "string.hpp"
#include "option.hpp"
#include "either.hpp"
#include "symbol.hpp"

namespace ax
{
    // The result of a parser.
    template<typename T>
    class parse : public either<std::string, T>
    {
    protected:

        const char* get_right_name() const override { return "parse_success"; }
        const char* get_left_name() const override { return "parse_failure"; }

    public:

        constraint(parse);
        template<typename A>
        using reify = parse<A>;
        using either<std::string, T>::either;
    };

    // Make a successful parse result.
    template<typename T>
    parse<T> parse_success(const T& t)
    {
        return parse<T>(t);
    }

    // Make a failed parse result.
    template<typename T>
    parse<T> parse_failure(const std::string& error_str)
    {
        return parse<T>(error_str, false);
    }

    // Make a successful parse result.
    template<typename T>
    parse<T> parse_success(T&& mvb)
    {
        return parse<T>(mvb);
    }

    // Make a failed parse result.
    template<typename T>
    parse<T> parse_failure(std::string&& error_str)
    {
        return parse<T>(error_str, false);
    }

    // Query that a parse is successful.
    template<typename T>
    bool is_parse_success(const parse<T>& parse)
    {
        return is_right(parse);
    }

    // Query that a parse is a failure.
    template<typename T>
    bool is_parse_failure(const parse<T>& parse)
    {
        return is_left(parse);
    }

    // Get the parse success value.
    template<typename T>
    const T& get_parse_success(const parse<T>& parse)
    {
        return get_right(parse);
    }

    // Get the parse failure value.
    template<typename T>
    const std::string& get_parse_failure(const parse<T>& parse)
    {
        return get_left(parse);
    }

    // Try to run a parse, rewinding the parsing iterator upon failure.
    template<typename T, typename P>
    parse<T> try_parse(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end, P parser)
    {
        val iter_copy = iter;
        val& parse = parser(iter, end);
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

    // Parse a symbol value from a std istream.
    parse<symbol> parse_symbol_from_stream(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end);

    // Parse a symbol value from an xml buffer.
    parse<symbol> parse_symbol_from_xml_buffer(char* buffer);
}

#endif
