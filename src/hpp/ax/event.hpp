#ifndef AX_EVENT_HPP
#define AX_EVENT_HPP

#include <cstddef>
#include <memory>

#include "prelude.hpp"
#include "address.hpp"

namespace ax
{
    // The event type.
    // TODO: consider if this should contain the addresses rather than the addressables...
    template<typename T>
    struct event final
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
        event(const event& that) = default;
        event(event&& that) = default;
        event& operator=(const event&) = default;
        event& operator=(event&&) = default;

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
