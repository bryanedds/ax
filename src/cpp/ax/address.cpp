#include "../../hpp/ax/impl/address.hpp"

#include "../../hpp/ax/impl/hash.hpp"
#include "../../hpp/ax/impl/string.hpp"
#include "../../hpp/ax/impl/vector.hpp"

namespace ax
{
    address::address(const ax::names& names) :
        hash_code(get_hash_range<ax::name>(names.begin(), names.end())),
        names(names) { }

    address::address(ax::names&& names) :
        hash_code(get_hash_range<ax::name>(names.begin(), names.end())),
        names(names) { }

    address::address(const ax::name& name) :
        hash_code(get_hash(name)),
        names({ name }) { }

    address::address(const ax::vector<std::string>& names) :
        address(std::transform<ax::names>(names.begin(), names.end(), [](VAL& name) { return ax::name(name); })) { }

    address::address(const std::vector<std::string>& names) :
        address(std::transform<ax::names>(names.begin(), names.end(), [](VAL& name) { return ax::name(name); })) { }

    address::address(const char* names_str) :
        address(std::string(names_str)) { }

    address::address(const std::string& names_str) :
        address(split_string(names_str, '/')) { }

    bool address::operator==(const ax::address& that) const
    {
        return
            hash_code == that.hash_code && // OPTIMIZATION: assuming hash_code check could hasten eq chacks
            names == that.names;
    }

    ax::address address::operator+(const ax::address& right) const
    {
        ax::names names_summed(names.begin(), names.end());
        for (const ax::name& name : right.names) names_summed.push_back(name);
        return ax::address(names_summed);
    }
}
