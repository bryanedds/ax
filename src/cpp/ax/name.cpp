#include "../../hpp/ax/impl/name.hpp"

#include "../../hpp/ax/impl/hash.hpp"

namespace ax
{
    name::name(const char* name_str) : name(std::string(name_str)) { }

    name::name(const std::string& name_str) : hash_code(get_hash(name_str)), name_str(name_str) { }

    name::name(std::string&& name_str) : hash_code(get_hash(name_str)), name_str(name_str) { }

    bool name::operator==(const ax::name& that) const { return name_str == that.name_str; }
}
