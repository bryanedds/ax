#ifndef AX_ADDRESSABLE_HPP
#define AX_ADDRESSABLE_HPP

#include <cstddef>

#include "prelude.hpp"
#include "castable.hpp"
#include "address.hpp"

namespace ax
{
    // A mixin for making a type addressable.
    class addressable : public ax::castable
    {
    public:

        CONSTRAINT(addressable);
        explicit addressable(const ax::address& address) : address(address) { }
        addressable(const ax::addressable& other) = default;
        addressable(ax::addressable&& other) = default;
        ax::addressable& operator=(const ax::addressable& other) = default;
        ax::addressable& operator=(ax::addressable&& other) = default;

        inline const ax::address& get_address() const { return address; }

    protected:

        ENABLE_CAST(ax::addressable, ax::castable);

    private:

        ax::address address;
    };
}

#endif
