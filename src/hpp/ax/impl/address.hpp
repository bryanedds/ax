#ifndef IMPL_AX_ADDRESS_HPP
#define IMPL_AX_ADDRESS_HPP

#include <cstddef>
#include <string>
#include <numeric>
#include <algorithm>

#include "prelude.hpp"
#include "string.hpp"
#include "name.hpp"
#include "vector.hpp"

namespace ax
{
    using names_t = ax::vector<ax::name_t, std::allocator<ax::name_t>, 3>;

    // The address of an event or a participant.
    class address
    {
    public:

        address() = default;
        address(const address& that) = default;
        address(address&& that) = default;
        address& operator=(const address&) = default;
        address& operator=(address&&) = default;

        explicit address(const ax::names_t& names);
        explicit address(ax::names_t&& names);
        explicit address(const ax::name_t& name);
        explicit address(const ax::vector<std::string>& names);
        explicit address(const char* names_str);
        explicit address(const std::string& names_str);
        bool operator==(const address& that) const;
        address operator+(const address& that) const;
        inline explicit operator std::size_t() const { return hash_code; }
        
        inline const std::string to_string() const
        {
            VAL& name_strs = std::transform<ax::vector<std::string>>(names.begin(), names.end(), [](const ax::name_t& name) { return name.to_string(); });
            return ax::join_strings(name_strs, '/');
        }

        inline const ax::names_t& get_names() const { return names; }

    private:

        std::size_t hash_code;
        ax::names_t names;
    };
}

namespace std
{
    inline std::string to_string(const ax::address& address)
    {
        return address.to_string();
    }

    template<>
    struct hash<ax::address>
    {
        std::size_t operator()(const ax::address& address) const
        {
            return static_cast<std::size_t>(address);
        }
    };

    template<>
    struct equal_to<ax::address>
    {
        std::size_t operator()(const ax::address& left, const ax::address& right) const
        {
            return left == right;
        }
    };
}

#endif
