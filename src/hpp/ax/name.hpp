#ifndef AX_NAME_HPP
#define AX_NAME_HPP

#include <cstddef>
#include <string>
#include <numeric>

#include "prelude.hpp"

namespace ax
{
    // A name value implemented as a data abstraction. Its hash is cached for true constant-time lookup.
    struct name final
    {
    public:

        name() = default;
        name(const ax::name&) = default;
        name(ax::name&&) = default;
        ax::name& operator=(const ax::name&) = default;
        ax::name& operator=(ax::name&&) = default;
        
        name(const char* name_str);
        name(const std::string& name_str);
        explicit name(std::string&& name_str);
        bool operator==(const ax::name& that) const;
        inline explicit operator std::size_t() const { return hash_code; }
        inline const std::string& to_string() const { return name_str; }

    private:

        std::size_t hash_code;
        std::string name_str;
    };
}

// Name suffix operator.
inline ax::name operator ""_n(const char *str, std::size_t len)
{
    return ax::name(std::string(str, len));
}

namespace std
{
    inline std::string to_string(const ax::name& name)
    {
        return name.to_string();
    }

    template<>
    struct hash<ax::name>
    {
        std::size_t operator()(const ax::name& name) const
        {
            return static_cast<std::size_t>(name);
        }
    };

    template<>
    struct equal_to<ax::name>
    {
        std::size_t operator()(const ax::name& left, const ax::name& right) const
        {
            return left == right;
        }
    };
}

#endif
