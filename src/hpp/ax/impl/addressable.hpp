#ifndef IMPL_AX_ADDRESSABLE_HPP
#define IMPL_AX_ADDRESSABLE_HPP

#include <cstddef>

#include "prelude.hpp"
#include "castable.hpp"
#include "address.hpp"

namespace ax
{
    // A mixin for making a type addressable.
    class addressable : public castable
    {
    public:

        CONSTRAINT(addressable);
        explicit addressable(const ax::address& address) : address(address) { }
        addressable(const addressable& other) = default;
        addressable(addressable&& other) = default;
        addressable& operator=(const addressable& other) = default;
        addressable& operator=(addressable&& other) = default;

        inline const ax::address& get_address() const { return address; }

    protected:

        ENABLE_CAST(addressable, castable);

    private:

        ax::address address;
    };
}

#endif
