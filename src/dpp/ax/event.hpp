#ifndef AX_EVENT_HPP
#define AX_EVENT_HPP

#include <cstddef>
#include <memory>

#include "prelude.hpp"
#include "address.hpp"

namespace ax
{
    // The event type. There is no need to make this an abstract data type for the same reason
    // there's no need to make a Matrix an abstract data type. Sometimes data is just data and
    // life is a breeze!
    template<typename T>
    struct event
    {
        constraint(event);

        template<typename A>
        using reify = event<A>;

        const T data;
        const address address;
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
            std::shared_ptr<addressable> subscriber,
            std::shared_ptr<addressable> publisher) :
            data(data),
            address(address),
            subscriber(subscriber),
            publisher(publisher) { }
    };
}

#endif