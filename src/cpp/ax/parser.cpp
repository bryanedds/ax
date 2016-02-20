#include <istream>

#include "../../hpp/rxml/rapidxml.hpp"

#include "../../hpp/ax/impl/parser.hpp"

namespace ax
{
    void skip_whitespace_many(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end)
    {
        while (iter != end && (*iter == ' ' || *iter == '\n' || *iter == '\r' || *iter == '\t'))
            ++iter;
    }

    parse<unit> skip_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end)
    {
        if (iter != end)
        {
            ++iter;
            return parse_success(unit());
        }
        return parse_failure<unit>("Parser error: expected any char.");
    }

    parse<unit> skip_given_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end, char chr)
    {
        if (iter != end)
        {
            if (*iter == chr)
            {
                ++iter;
                return parse_success(unit());
            }
            return parse_failure<unit>("Parser error: expected char.");
        }
        return parse_failure<unit>("Parser error: expected char.");
    }

    parse<char> parse_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end)
    {
        if (iter != end)
        {
            VAL chr = *iter;
            ++iter;
            return parse_success(chr);
        }
        return parse_failure<char>("Parser error: expected char.");
    }

    parse<std::string> parse_until_given_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end, char chr)
    {
        VAR success = false;
        std::string str_mvb{};
        while (iter != end)
        {
            if (*iter != chr)
            {
                str_mvb.push_back(*iter);
                ++iter;
            }
            else
            {
                success = true;
                break;
            }
        }
        if (success) return parse_success(std::move(str_mvb));
        return parse_failure<std::string>("Parser error: expected char.");
    }

    parse<std::string> parse_until_any_given_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end, std::string chars)
    {
        VAR success = false;
        std::string str_mvb{};
        while (iter != end)
        {
            VAL chr = *iter;
            if (chars.find(chr, 0_z) == std::string::npos)
            {
                str_mvb.push_back(chr);
                ++iter;
            }
            else
            {
                success = true;
                break;
            }
        }
        if (success) return parse_success(std::move(str_mvb));
        return parse_failure<std::string>("Parser error: expected any char.");
    }

    parse<symbol> parse_symbol_from_stream(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end)
    {
        skip_whitespace_many(iter, end);
        if (VAL& parse = parse_char(iter, end))
        {
            char chr = *parse;
            switch (chr)
            {
                case '[':
                {
                    skip_whitespace_many(iter, end);
                    std::vector<symbol> children_mvb{};
                    while (iter != end && *iter != ']')
                    {
                        if (VAL& parse = parse_symbol_from_stream(iter, end)) children_mvb.push_back(*parse);
                        else return parse_failure<symbol>(~parse);
                        skip_whitespace_many(iter, end);
                    }
                    if (VAL& parse = skip_given_char(iter, end, ']')) return parse_success(symbols(std::move(children_mvb)));
                    else return parse_failure<symbol>(~parse);
                }
                case '\"':
                {
                    if (VAL& parse = parse_until_given_char(iter, end, '\"'))
                    {
                        if (VAL& parse2 = skip_given_char(iter, end, '\"')) return parse_success(atom(*parse));
                        else return parse_failure<symbol>(~parse2);
                    }
                    else return parse_failure<symbol>(~parse);
                }
                default:
                {
                    if (VAL& parse = parse_until_any_given_char(iter, end, "[]\" \n\r\t")) return parse_success(atom(std::string(1, chr) + *parse));
                    else return parse_failure<symbol>(~parse);
                }
            }
        }
        return parse_failure<symbol>("Unknown parser error.");
    }

    static symbol parse_symbol_from_xml_node(rapidxml::xml_node<char>* parent_node)
    {
        symbol::right_type symbols{};
        for (VAR* node = parent_node->first_node(); node != nullptr; node = node->next_sibling())
        {
            symbols.emplace_back(atom(node->name()));
            for (VAR* attribute = node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                symbols.emplace_back(atom(attribute->value()));
            symbols.emplace_back(parse_symbol_from_xml_node(node));
        }
        return ax::symbols(std::move(symbols));
    }

    parse<symbol> parse_symbol_from_xml_buffer(char* buffer)
    {
        rapidxml::xml_document<char> document{};
        document.parse<rapidxml::parse_default>(buffer);
        VAR* root = document.first_node();
        return parse_success(parse_symbol_from_xml_node(root));
    }
}
