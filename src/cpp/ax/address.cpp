#include "../../hpp/ax/impl/address.hpp"

#include "../../hpp/ax/impl/hash.hpp"
#include "../../hpp/ax/impl/string.hpp"

namespace ax
{
    address::address(const ax::name_t& name) :
        hash_code(get_hash(name)),
        names({ name }) { }

    address::address(const ax::names_t& names) :
        hash_code(get_hash_range<ax::name_t>(names.begin(), names.end())),
        names(names) { }

    address::address(ax::names_t&& names) :
        hash_code(get_hash_range<ax::name_t>(names.begin(), names.end())),
        names(names) { }

    address::address(const std::vector<ax::name_t>& names) :
        hash_code(get_hash_range<ax::name_t>(names.begin(), names.end())),
        names(names) { }

    address::address(std::vector<ax::name_t>&& names) :
        hash_code(get_hash_range<ax::name_t>(names.begin(), names.end())),
        names(names) { }

    address::address(const std::vector<std::string>& names) :
        address(std::transform<std::vector<ax::name_t>>(names.begin(), names.end(), [](VAL& name) { return ax::name_t(name); })) { }

    address::address(const char* names_str) :
        address(std::string(names_str)) { }

    address::address(const std::string& names_str) :
        address(split_string(names_str, '/')) { }

    bool address::operator==(const address& that) const
    {
        return
            hash_code == that.hash_code && // OPTIMIZATION: assuming hash_code check could hasten eq chacks
            names == that.names;
    }

    address address::operator+(const address& right) const
    {
        std::vector<ax::name_t> names_summed(names.begin(), names.end());
        for (const ax::name_t& name : right.names) names_summed.push_back(name);
        return address(names_summed);
    }
}
