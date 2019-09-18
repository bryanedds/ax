#include <istream>
#include <sstream>

#include "../../hpp/rxml/rapidxml.hpp"

#include "../../hpp/ax/impl/parser.hpp"

namespace ax
{
    void skip_whitespace_many(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end)
    {
        while (iter != end && (*iter == ' ' || *iter == '\n' || *iter == '\r' || *iter == '\t'))
            ++iter;
    }

    ax::parse<unit> skip_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end)
    {
        if (iter != end)
        {
            ++iter;
            return ax::parse_success(ax::unit());
        }
        return ax::parse_failure<ax::unit>("Parser error: expected any char.");
    }

    ax::parse<ax::unit> skip_given_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end, char chr)
    {
        if (iter != end)
        {
            if (*iter == chr)
            {
                ++iter;
                return ax::parse_success(ax::unit());
            }
            return ax::parse_failure<ax::unit>("Parser error: expected char.");
        }
        return ax::parse_failure<ax::unit>("Parser error: expected char.");
    }

    parse<char> parse_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end)
    {
        if (iter != end)
        {
            VAL chr = *iter;
            ++iter;
            return ax::parse_success(chr);
        }
        return ax::parse_failure<char>("Parser error: expected char.");
    }

    ax::parse<std::string> parse_until_given_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end, char chr)
    {
        VAR success = false;
        std::string str_mvb;
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
        if (success) return ax::parse_success(std::move(str_mvb));
        return ax::parse_failure<std::string>("Parser error: expected char.");
    }

    ax::parse<std::string> parse_until_any_given_char(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end, std::string chars)
    {
        VAR success = false;
        std::string str_mvb;
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
        if (success) return ax::parse_success(std::move(str_mvb));
        return ax::parse_failure<std::string>("Parser error: expected any char.");
    }

    static ax::symbol parse_symbol_from_xml_node(rapidxml::xml_node<char>* parent_node)
    {
        ax::symbols_t symbols;
        for (VAR* node = parent_node->first_node(); node != nullptr; node = node->next_sibling())
        {
            symbols.push_back(ax::atom(node->name()));
            for (VAR* attribute = node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                symbols.push_back(ax::atom(attribute->value()));
            symbols.push_back(ax::parse_symbol_from_xml_node(node));
        }
        return ax::symbols(std::move(symbols));
    }

    ax::parse<ax::symbol> parse_symbol_from_xml_buffer(char* buffer)
    {
        rapidxml::xml_document<char> document;
        document.parse<rapidxml::parse_default>(buffer);
        VAR* root = document.first_node();
        return ax::parse_success(ax::parse_symbol_from_xml_node(root));
    }

    ax::parse<ax::symbol> parse_symbol_from_stream(std::istream_iterator<char>& iter, const std::istream_iterator<char>& end)
    {
        ax::skip_whitespace_many(iter, end);
        if (VAL& parse = ax::parse_char(iter, end))
        {
            char chr = *parse;
            switch (chr)
            {
                case '[':
                {
                    ax::skip_whitespace_many(iter, end);
                    std::vector<ax::symbol> children_mvb;
                    while (iter != end && *iter != ']')
                    {
                        if (VAL& parse = ax::parse_symbol_from_stream(iter, end)) children_mvb.push_back(*parse);
                        else return ax::parse_failure<ax::symbol>(~parse);
                        ax::skip_whitespace_many(iter, end);
                    }
                    if (VAL& parse = ax::skip_given_char(iter, end, ']')) return ax::parse_success(ax::symbols(std::move(children_mvb)));
                    else return ax::parse_failure<ax::symbol>(~parse);
                }
                case '\"':
                {
                    if (VAL& parse = ax::parse_until_given_char(iter, end, '\"'))
                    {
                        if (VAL& parse2 = ax::skip_given_char(iter, end, '\"')) return ax::parse_success(ax::atom(*parse));
                        else return ax::parse_failure<ax::symbol>(~parse2);
                    }
                    else return ax::parse_failure<ax::symbol>(~parse);
                }
                case '`':
                {
                    // TODO: implement recursive quotes
                    if (VAL& parse = ax::parse_until_given_char(iter, end, '\''))
                    {
                        if (VAL& parse2 = ax::skip_given_char(iter, end, '\'')) return ax::parse_success(ax::quote(*parse));
                        else return ax::parse_failure<ax::symbol>(~parse2);
                    }
                    else return ax::parse_failure<ax::symbol>(~parse);
                }
                default:
                {
                    if (VAL& parse = ax::parse_until_any_given_char(iter, end, "[]`\'\" \n\r\t")) return ax::parse_success(ax::atom(std::string(1, chr) + *parse));
                    else return ax::parse_failure<ax::symbol>(~parse);
                }
            }
        }
        else return ax::parse_failure<ax::symbol>("Unknown parser error due to: "_s + ~parse);
    }

    ax::parse<ax::symbol> parse_symbol(const std::string& str)
    {
        std::stringstream sstr(str);
        sstr << std::noskipws; // apparently avoids skipping whitespace
        std::istream_iterator<char> iter(sstr);
        return ax::parse_symbol_from_stream(iter, std::istream_iterator<char>());
    }
}
