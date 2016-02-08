#include "../../hpp/ax/address.hpp"

namespace ax
{
    bool address::operator==(const address& that) const
    {
        return
            hash_code == that.hash_code && // OPTIMIZATION: assuming hash_code check could hasten eq chacks
            names == that.names;
    }

    address address::operator+(const address& right) const
    {
        std::vector<name_t> names_summed(names.cbegin(), names.cend());
        for (Val& name : right.names) names_summed.push_back(name);
        return address(names_summed);
    }

    const std::vector<name_t>& get_names(const address& address)
    {
        return address.names;
    }
}
