#include "../../hpp/rxml/rapidxml.hpp"

#include "../../hpp/ax/parser.hpp"

namespace ax
{
    void skip_whitespace_many(std::string::const_iterator& iter, const std::string::const_iterator& end)
    {
        while (iter != end && (*iter == ' ' || *iter == '\n' || *iter == '\r' || *iter == '\t'))
            ++iter;
    }

    parse<unit> skip_char(std::string::const_iterator& iter, const std::string::const_iterator& end)
    {
        if (iter != end)
        {
            ++iter;
            return parse_success(unit());
        }
        return parse_failure<unit>("Parser error: expected any char.");
    }

    parse<unit> skip_given_char(std::string::const_iterator& iter, const std::string::const_iterator& end, char chr)
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

    parse<char> parse_char(std::string::const_iterator& iter, const std::string::const_iterator& end)
    {
        if (iter != end)
        {
            val chr = *iter;
            ++iter;
            return parse_success(chr);
        }
        return parse_failure<char>("Parser error: expected char.");
    }

    parse<std::string> parse_until_given_char(std::string::const_iterator& iter, const std::string::const_iterator& end, char chr)
    {
        var success = false;
        std::string str_mvb{};
        while (iter != end)
        {
            if (*iter != chr)
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
        return parse_failure<std::string>("Parser error: expected char.");
    }

    parse<std::string> parse_until_any_given_char(std::string::const_iterator& iter, const std::string::const_iterator& end, std::string chars)
    {
        var success = false;
        std::string str_mvb{};
        while (iter != end)
        {
            val chr = *iter;
            if (chars.find(chr, 0_z) != std::string::npos)
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

    parse<symbol> parse_symbol_from_string(std::string::const_iterator& iter, const std::string::const_iterator& end)
    {
        skip_whitespace_many(iter, end);
        if (val& parse = parse_char(iter, end))
        {
            switch (*parse)
            {
                case '[':
                {
                    skip_whitespace_many(iter, end);
                    std::vector<symbol> children_mvb{};
                    while (iter != end)
                    {
                        if (val& parse = parse_symbol_from_string(iter, end)) children_mvb.push_back(*parse);
                        else return parse_failure<symbol>(~parse);
                        skip_whitespace_many(iter, end);
                    }
                    if (val& parse = skip_given_char(iter, end, ']')) return parse_success(symbol_tree(std::move(children_mvb)));
                    else return parse_failure<symbol>(~parse);
                }
                default:
                {
                    if (val& parse = parse_until_any_given_char(iter, end, "[] \n\r\t")) return parse_success(symbol_leaf(*parse));
                    else return parse_failure<symbol>(~parse);
                }
            }
        }
        return parse_failure<symbol>("Unknown parser error.");
    }

    static symbol parse_symbol_from_xml_node(rapidxml::xml_node<char>* parent_node)
    {
        symbol::right_type symbol_tree{};
        for (var* node = parent_node->first_node(); node != nullptr; node = node->next_sibling())
        {
            symbol_tree.emplace_back(symbol_leaf(node->name()));
            for (var* attribute = node->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
                symbol_tree.emplace_back(symbol_leaf(attribute->value()));
            symbol_tree.emplace_back(parse_symbol_from_xml_node(node));
        }
        return ax::symbol_tree(std::move(symbol_tree));
    }

    parse<symbol> parse_symbol_from_xml_buffer(char* buffer)
    {
        rapidxml::xml_document<char> document{};
        document.parse<rapidxml::parse_default>(buffer);
        var* root = document.first_node();
        return parse_success(parse_symbol_from_xml_node(root));
    }
}
