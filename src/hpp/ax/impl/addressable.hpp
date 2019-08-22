#ifndef IMPL_AX_ADDRESSABLE_HPP
#define IMPL_AX_ADDRESSABLE_HPP

#include <cstddef>

#include "prelude.hpp"
#include "castable.hpp"
#include "address.hpp"

namespace ax
{
    // A mixin for making a type addressable.
    // TODO: make this reflectable?
    class addressable : public castable
    {
    public:

        CONSTRAINT(addressable);
        explicit addressable(const ax::address& address) : address(address) { }
        inline const ax::address& get_address() const { return address; }

    protected:

        ENABLE_CAST(addressable, castable);

    private:

        address address;
    };
}

#endif
