#ifndef IMPL_AX_EVENTABLE_HPP
#define IMPL_AX_EVENTABLE_HPP

#include <cstddef>
#include <functional>
#include <string>
#include <memory>
#include <unordered_map>

#include "prelude.hpp"
#include "id.hpp"
#include "castable.hpp"
#include "address.hpp"
#include "addressable.hpp"
#include "subscription.hpp"

namespace ax
{
    template<typename P>
    using unsubscriber = std::function<void(P&)>;

    // A program mixin for enabling publisher-neutral events in a program. What is a program mixin?
    // Well, it's like any other C++ mixin, except it's intended for use on the type that end-user
    // will represent his program with. Program mixins are the good alternative to OOP Singletons.
    template<typename P>
    class eventable : public ax::castable
    {
    public:

        CONSTRAINT(eventable);

        eventable() :
            pred_id(std::make_unique<ax::id_t>())
        {
            CONSTRAIN(P, eventable);
            program = dynamic_cast<P*>(this);
        }

        ax::id_t get_subscription_id()
        {
            VAL& pred_id = *this->pred_id;
            VAL& succ_id = succ(pred_id);
            this->pred_id = std::make_unique<ax::id_t>(succ_id);
            return succ_id;
        }

        void unsubscribe_event(ax::id_t subscription_id)
        {
            VAL& unsubscription_opt = unsubscription_map.find(subscription_id);
            if (unsubscription_opt != std::end(unsubscription_map))
            {
                VAL& subscriptions_opt = subscriptions_map.find(unsubscription_opt->second.first);
                if (subscriptions_opt != std::end(subscriptions_map))
                {
                    VAR& subscriptions = *subscriptions_opt->second;
                    subscriptions.erase(
                        std::remove_if(
                        std::begin(subscriptions),
                        std::end(subscriptions),
                        [unsubscription_opt](VAL& subscription)
                    { return subscription->subscriber_opt.lock().get() == unsubscription_opt->second.second.lock().get(); }));
                    unsubscription_map.erase(unsubscription_opt);
                }
            }
        }

        template<typename T, typename H>
        ax::unsubscriber<P> subscribe_event5(ax::id_t subscription_id, const ax::address& address, const std::shared_ptr<ax::addressable>& subscriber, const H& handler)
        {
            VAR subscription_detail_mvb = cast<ax::castable>(std::make_unique<ax::subscription_detail<T, P>>(handler));
            VAR subscriptions_opt = subscriptions_map.find(address);
            if (subscriptions_opt != std::end(subscriptions_map))
            {
                VAL& subscription = std::make_shared<ax::subscription>(subscription_id, subscriber, std::move(subscription_detail_mvb));
                subscriptions_opt->second->push_back(subscription);
            }
            else
            {
                VAL& subscription = std::make_shared<ax::subscription>(subscription_id, subscriber, std::move(subscription_detail_mvb));
                VAR subscriptions_mvb = std::make_unique<ax::subscription_list>();
                subscriptions_mvb->push_back(subscription);
                subscriptions_map.insert(std::make_pair(address, std::move(subscriptions_mvb)));
            }
            unsubscription_map.insert(std::make_pair(subscription_id, std::make_pair(address, subscriber)));
            return [subscription_id](P& program) { program.unsubscribe_event(subscription_id); };
        }

        template<typename T, typename H>
        ax::unsubscriber<P> subscribe_event(const ax::address& address, const std::shared_ptr<ax::addressable>& subscriber, const H& handler)
        {
            return subscribe_event5<T>(get_subscription_id(), address, subscriber, handler);
        }

        template<typename T>
        void publish_event(const T& event_data, const ax::address& event_address, const std::shared_ptr<ax::addressable>& publisher)
        {
            VAL& subscriptions_opt = subscriptions_map.find(event_address);
            if (subscriptions_opt != std::end(subscriptions_map))
            {
                VAL subscriptions_copy = *subscriptions_opt->second;
                for (VAL& subscription : subscriptions_copy)
                {
                    VAL cascade = subscription->template publish_subscription<T, P>(event_data, event_address, publisher, *program);
                    if (!cascade) break;
                }
            }
        }

    protected:

        ENABLE_CAST(ax::eventable<P>, ax::castable);

    private:

        P* program;
        std::unique_ptr<ax::id_t> pred_id;
        ax::subscriptions_map subscriptions_map;
        ax::unsubscription_map unsubscription_map;
    };
}

#endif
