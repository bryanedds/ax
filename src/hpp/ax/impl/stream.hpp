#ifndef IMPL_AX_STREAM_HPP
#define IMPL_AX_STREAM_HPP

#include <cstddef>
#include <functional>
#include <string>
#include <memory>

#include "../../crossguid/Guid.hpp"

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

    template<typename W>
    struct subscribe
    {
        ax::address address;
        ax::unsubscribe<W> unsubscribe;
    };

    template<typename T, typename W>
    struct stream
    {
        stream make_empty()
        {
            CONSTRAIN(W, ax::eventable);
            VAL subscribe = [](W& w) { return ax::subscribe { address{}, [](W&){} }; }
            return stream { subscribe };
        }

        stream make(ax::address event_address)
        {
            CONSTRAIN(W, ax::eventable);
            VAL& subscribe = [event_address](W& w)
            {
                VAL& global_participant = w.get_global_participant();
                VAL& subscription_key = xg::newGuid(); // TODO: make ax::make_guid();
                VAL& subscription_address = ax::address(ax::name(std::to_string(subscription_key)));
                VAL& unsubscribe = [](W& w) { return w.unsubscribe<W>(subscription_key); };
                VAL& subscription = [](const ax::event<T>& evt)
                {
                    // TODO: event tracing
                    // TODO: subscription sorting
                    // TODO: allow wildcard flag (= false)
                    // TODO: event cascading (= cascade)
                    w.publish_event<T>(evt.data, subscription_address, global_participant);
                };
                w.subscribe_event<T>(subscription_key, event_address, global_participant);
                return ax::subscribe{ subscription_address, unsubscribe };
            }
            return ax::stream{ subscribe };
        }

        std::function<ax::subscribe(W&)>;
    };
}

#endif
