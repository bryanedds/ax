#ifndef AX_NAME_HPP
#define AX_NAME_HPP

#include <cstddef>
#include <string>
#include <numeric>
#include <vector>

#include "prelude.hpp"
#include "hash.hpp"

namespace ax
{
    // A name value implemented as a data abstraction. Its hash is cached for true constant-time
    // lookup.
    class name_t
    {
    private:

        std::size_t hash_code;
        std::string name_str;

    protected:

        friend const std::string& get_name_str(const name_t& name);

    public:

        name_t() = default;
        name_t(const name_t&) = default;
        name_t(name_t&&) = default;
        name_t& operator=(const name_t&) = default;
        name_t& operator=(name_t&&) = default;
        
        name_t(const char* name_str) : name_t(std::string(name_str)) { }
        name_t(const std::string& name_str) : hash_code(get_hash(name_str)), name_str(name_str) { }
        explicit name_t(std::string&& name_str_mvb) : hash_code(get_hash(name_str_mvb)), name_str(std::move(name_str_mvb)) { }
        explicit operator std::size_t() const { return hash_code; }
        bool operator==(const name_t& that) const { return name_str == that.name_str; }
    };

    // Get the string of which a name is composed.
    const std::string& get_name_str(const name_t& name);
}

// Name suffix operator.
inline ax::name_t operator ""_n(const char *str, std::size_t len)
{
    return ax::name_t(std::string(str, len));
}

namespace std
{
    template<>
    struct hash<ax::name_t>
    {
        std::size_t operator()(const ax::name_t& name) const
        {
            return static_cast<std::size_t>(name);
        }
    };

    template<>
    struct equal_to<ax::name_t>
    {
        std::size_t operator()(const ax::name_t& left, const ax::name_t& right) const
        {
            return left == right;
        }
    };
}

#endif
