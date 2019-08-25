#ifndef IMPL_AX_ID_HPP
#define IMPL_AX_ID_HPP

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <limits>

#include "prelude.hpp"

namespace ax
{
    // A unique, counting identifier.
    // Intended to be passed by value.
    // Not intended to be serialized.
    class id_t
    {
    public:
        
        constexpr id_t() : x(zero<int64_t>()), y(zero<int64_t>()) { }
        constexpr id_t(const ax::id_t&) = default;
        constexpr id_t(ax::id_t&&) = default;
        ax::id_t& operator=(const ax::id_t&) = default;
        ax::id_t& operator=(ax::id_t&&) = default;
        
        constexpr id_t(int64_t x, int64_t y) : x(x), y(y) { }
        constexpr bool operator==(const id_t& that) const { return x == that.x && y == that.y; }
        static constexpr ax::id_t invalid() { return ax::id_t(); }
        explicit operator std::size_t() const { return static_cast<std::size_t>(x ^ y); }

        ax::id_t succ()
        {
            if (x == std::numeric_limits<int64_t>::max())
            {
                if (y == std::numeric_limits<int64_t>::max()) throw std::out_of_range("ax::id_t overflowed.");
                return ax::id_t(zero<int64_t>(), ax::succ(y));
            }
            return ax::id_t(ax::succ(x), y);
        }

        const int64_t x;
        const int64_t y;
    };

    template<>
    constexpr ax::id_t zero<ax::id_t>()
    {
        return id_t::invalid();
    }

    template<>
    inline ax::id_t succ<ax::id_t>(ax::id_t id)
    {
        return id.succ();
    }
}

namespace std
{
    template<>
    struct hash<ax::id_t>
    {
        std::size_t operator()(const ax::id_t& id) const
        {
            return static_cast<std::size_t>(id);
        }
    };

    template<>
    struct equal_to<ax::id_t>
    {
        std::size_t operator()(const ax::id_t& left, const ax::id_t& right) const
        {
            return left == right;
        }
    };
}

#endif
