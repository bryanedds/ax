#ifndef IMPL_AX_EVENT_HPP
#define IMPL_AX_EVENT_HPP

#include <cstddef>
#include <memory>

#include "prelude.hpp"
#include "address.hpp"

namespace ax
{
    // The event type.
    template<typename T>
    class event
    {
    public:

        CONSTRAINT(event);

        template<typename A>
        using reify = ax::event<A>;

        const T data;
        const ax::address address;
        const std::shared_ptr<addressable> subscriber; // TODO: may be necessary to genericize subscriber
        const std::shared_ptr<addressable> publisher;
        
        event() = delete;
        event(const ax::event& that) = default;
        event(ax::event&& that) = default;
        ax::event& operator=(const ax::event&) = default;
        ax::event& operator=(ax::event&&) = default;

        event(
            const T& data,
            const ax::address& address,
            std::shared_ptr<ax::addressable> subscriber,
            std::shared_ptr<ax::addressable> publisher) :
            data(data),
            address(address),
            subscriber(subscriber),
            publisher(publisher) { }
    };
}

#endif
