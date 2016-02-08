#include "../../impl/ax/name.hpp"

#include "../../impl/ax/hash.hpp"

namespace ax
{

    name_t::name_t(const char* name_str) : name_t(std::string(name_str)) { }

    name_t::name_t(const std::string& name_str) : hash_code(get_hash(name_str)), name_str(name_str) { }

    name_t::name_t(std::string&& name_str_mvb) : hash_code(get_hash(name_str_mvb)), name_str(std::move(name_str_mvb)) { }

    name_t::operator std::size_t() const
    {
        return hash_code;
    }

    bool name_t::operator==(const name_t& that) const
    {
        return name_str == that.name_str;
    }

    const std::string& get_name_str(const name_t& name)
    {
        return name.name_str;
    }
}
