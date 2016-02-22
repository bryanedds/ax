#ifndef IMPL_AX_NAME_HPP
#define IMPL_AX_NAME_HPP

#include <cstddef>
#include <string>
#include <numeric>

#include "prelude.hpp"

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
        
        name_t(const char* name_str);
        name_t(const std::string& name_str);
        explicit name_t(std::string&& name_str);
        explicit operator std::size_t() const;
        bool operator==(const name_t& that) const;
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
