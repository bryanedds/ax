#ifndef IMPL_AX_STREAM_HPP
#define IMPL_AX_STREAM_HPP

#include <cstddef>
#include <functional>
#include <string>
#include <memory>

#include "../../crossguid/Guid.hpp" // TODO: "guid.hpp" instead

#include "prelude.hpp"
#include "id.hpp"
#include "castable.hpp"
#include "address.hpp"
#include "addressable.hpp"
#include "eventable.hpp"

namespace ax
{
    template<typename W>
    using unsubscribe = std::function<void(W&)> fn;

    template<typename T, typename W>
    struct subscribe
    {
        ax::address address;
        ax::unsubscribe<W> unsubscribe;
    };

    template<typename T, typename W>
    using stream = std::function<ax::subscribe(W&)>;

    template<typename T, typename W>
    stream<T, W> stream_make_empty()
    {
        CONSTRAIN(W, ax::eventable<W>);
        VAL subscribe = [](W& w) { return ax::subscribe{ address{}, [](W&) {} }; }
        return ax::stream<T, W>{ subscribe };
    }

    template<typename T, typename W>
    stream<T, W> stream_make(ax::address event_address)
    {
        CONSTRAIN(W, ax::eventable<W>);
        VAL& subscribe = [event_address](W& w)
        {
            VAL& global_participant = w.get_global_participant();
            VAL& subscription_key = xg::newGuid();
            VAL& subscription_address = ax::address(ax::name(std::to_string(subscription_key)));
            VAL& unsubscribe = [](W& w) { return w.unsubscribe<W>(subscription_key); };
            VAL& subscription = [](const ax::event<T>& evt)
            {
                // TODO: event tracing
                // TODO: subscription sorting
                // TODO: allow wildcard flag (default false)
                // TODO: event cascading (default cascade)
                w.publish_event<T>(evt.data, subscription_address, global_participant);
            };
            w.subscribe_event<T>(subscription_key, event_address, global_participant, subscription);
            return ax::subscribe<W>{ subscription_address, unsubscribe };
        };
        return ax::stream<T, W>{ subscribe };
    }

    template<typename T, typename W>
    ax::subscribe<T, W> stream_subscribe(ax::subscription subscription, const std::shared_ptr<ax::addressable> subscriber, ax::stream<T, W> stream, W& w)
    {
        CONSTRAIN(W, ax::eventable<W>);
        VAL& subscribe = [subscription, subscriber, stream](W& w)
        {
            VAL& subscription_key = xg::newGuid();
            VAL& subscription_address = ax::address(ax::name(std::to_string(subscription_key)));
            VAL& subscribe = stream(w);
            VAL& address = subscribe.address;
            VAL& unsubscribe = subscribe.unsubscribe;
            VAL& unsubscribe = [subscription_key, unsubscribe](W& w)
            {
                unsubscribe(w);
                w.unsubscribe(subscription_key, w);
            };
            w.subscribe_event<T>(subscription_key, event_address, subscriber, subscription);
            return ax::subscribe<T, W>{ subscription_address, unsubscribe };
        };
        VAL& stream = ax::stream{ subscribe };
        return stream(w);
    }
}

#endif
