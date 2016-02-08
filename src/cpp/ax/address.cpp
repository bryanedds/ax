#include "../../impl/ax/address.hpp"

#include "../../impl/ax/hash.hpp"
#include "../../impl/ax/string.hpp"

namespace ax
{
    address::address(const name_t& name) : hash_code(get_hash(name)), names({ name }) { }

    address::address(const std::vector<name_t>& names) : hash_code(get_hash_range<name_t>(names.cbegin(), names.cend())), names(names) { }

    address::address(std::vector<name_t>&& names_mvb) : hash_code(get_hash_range<name_t>(names_mvb.cbegin(), names_mvb.cend())), names(names_mvb) { }

    address::address(const std::vector<std::string>& names) : address(std::transform<std::vector<name_t>>(names.cbegin(), names.cend(), [](Val& name) { return name_t(name); })) { }

    address::address(const char* names_str) : address(std::string(names_str)) { }

    address::address(const std::string& names_str) : address(split_string(names_str, '/')) { }

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

    address::operator std::size_t() const
    {
        return hash_code;
    }

    const std::vector<name_t>& get_names(const address& address)
    {
        return address.names;
    }
}
