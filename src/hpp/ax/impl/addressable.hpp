#ifndef IMPL_AX_ADDRESSABLE_HPP
#define IMPL_AX_ADDRESSABLE_HPP

#include <cstddef>

#include "prelude.hpp"
#include "castable.hpp"
#include "address.hpp"

namespace ax
{
    // A mixin for making a type addressable.
    //
    // The policy hard-wired into this mixin is that an addressable's name cannot change. If an
    // addressable needs a different name, the policy is to copy it with a different name, then
    // discard the original. This actually works best in simulators because trying to implement
    // mutable identities is overly complicating in practice, and in theory actually makes no
    // sense anyways.
    //
    // TODO: make this reflectable.
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
