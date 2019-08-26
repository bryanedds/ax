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
    struct id final
    {
    public:
        
        constexpr id() : x(ax::zero<int64_t>()), y(zero<int64_t>()) { }
        constexpr id(const ax::id&) = default;
        constexpr id(ax::id&&) = default;
        ax::id& operator=(const ax::id&) = default;
        ax::id& operator=(ax::id&&) = default;
        
        constexpr id(int64_t x, int64_t y) : x(x), y(y) { }
        constexpr bool operator==(const id& that) const { return x == that.x && y == that.y; }
        static constexpr ax::id invalid() { return ax::id(); }
        explicit operator std::size_t() const { return static_cast<std::size_t>(x ^ y); }

        ax::id succ()
        {
            if (x == std::numeric_limits<int64_t>::max())
            {
                if (y == std::numeric_limits<int64_t>::max()) throw std::out_of_range("ax::id overflowed.");
                return ax::id(zero<int64_t>(), ax::succ(y));
            }
            return ax::id(ax::succ(x), y);
        }

        const int64_t x;
        const int64_t y;
    };

    template<>
    constexpr ax::id zero<ax::id>()
    {
        return ax::id::invalid();
    }

    template<>
    inline ax::id succ<ax::id>(ax::id id)
    {
        return id.succ();
    }
}

namespace std
{
    template<>
    struct hash<ax::id>
    {
        std::size_t operator()(const ax::id& id) const
        {
            return static_cast<std::size_t>(id);
        }
    };

    template<>
    struct equal_to<ax::id>
    {
        std::size_t operator()(const ax::id& left, const ax::id& right) const
        {
            return left == right;
        }
    };
}

#endif
