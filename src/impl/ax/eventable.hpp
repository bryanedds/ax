#ifndef IMPL_AX_EVENTABLE_HPP
#define IMPL_AX_EVENTABLE_HPP

#include <cstddef>
#include <functional>
#include <unordered_map>
#include <memory>

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
    class eventable : public castable
    {
    private:

        std::unique_ptr<id_t> pred_id;
        ax::subscriptions_map subscriptions_map;
        ax::unsubscription_map unsubscription_map;

    protected:

        enable_cast(eventable<P>, castable);

        template<typename A>
        friend id_t get_subscription_id(A& program);

        template<typename A>
        friend void unsubscribe_event(A& program, id_t subscription_id);

        template<typename U, typename A, typename H>
        friend unsubscriber<A> subscribe_event5(A& program, id_t subscription_id, const address& address, const std::shared_ptr<addressable>& subscriber, const H& handler);

        template<typename U, typename A>
        friend void publish_event(A& program, const U& event_data, const address& address, const std::shared_ptr<addressable>& publisher);

    public:

        Constraint(eventable);
        eventable() : pred_id(std::make_unique<id_t>()) { }
    };

    template<typename P>
    id_t get_subscription_id(P& program)
    {
        Constrain(P, eventable);
        Val& pred_id = *program.pred_id;
        Val& succ_id = succ(pred_id);
        program.pred_id = std::make_unique<id_t>(succ_id);
        return succ_id;
    }

    template<typename P>
    void unsubscribe_event(P& program, id_t subscription_id)
    {
        Constrain(P, eventable);
        Val& unsubscription_opt = program.unsubscription_map.find(subscription_id);
        if (unsubscription_opt != std::end(program.unsubscription_map))
        {
            Val& subscriptions_opt = program.subscriptions_map.find(unsubscription_opt->second.first);
            if (subscriptions_opt != std::end(program.subscriptions_map))
            {
                Var& subscriptions = *subscriptions_opt->second;
                subscriptions.erase(
                    std::remove_if(
                        std::begin(subscriptions),
                        std::end(subscriptions),
                        [unsubscription_opt](Val& subscription)
                        { return subscription->subscriber_opt.lock().get() == unsubscription_opt->second.second.lock().get(); }));
                program.unsubscription_map.erase(unsubscription_opt);
            }
        }
    }

    template<typename T, typename P, typename H>
    unsubscriber<P> subscribe_event5(P& program, id_t subscription_id, const address& address, const std::shared_ptr<addressable>& subscriber, const H& handler)
    {
        Constrain(P, eventable);
        Var subscription_detail_mvb = cast<castable>(std::make_unique<subscription_detail<T, P>>(handler));
        Var subscriptions_opt = program.subscriptions_map.find(address);
        if (subscriptions_opt != std::end(program.subscriptions_map))
        {
            Val& subscription = std::make_shared<ax::subscription>(subscription_id, subscriber, std::move(subscription_detail_mvb));
            subscriptions_opt->second->push_back(subscription);
        }
        else
        {
            Val& subscription = std::make_shared<ax::subscription>(subscription_id, subscriber, std::move(subscription_detail_mvb));
            Var subscriptions_mvb = std::make_unique<subscription_list>();
            subscriptions_mvb->push_back(subscription);
            program.subscriptions_map.insert(std::make_pair(address, std::move(subscriptions_mvb)));
        }
        program.unsubscription_map.insert(std::make_pair(subscription_id, std::make_pair(address, subscriber)));
        return [subscription_id](P& program){ unsubscribe_event(program, subscription_id); };
    }

    template<typename T, typename P, typename H>
    unsubscriber<P> subscribe_event(P& program, const address& address, const std::shared_ptr<addressable>& subscriber, const H& handler)
    {
        Constrain(P, eventable);
        return subscribe_event5<T, P>(program, get_subscription_id(program), address, subscriber, handler);
    }

    template<typename T, typename P>
    void publish_event(P& program, const T& event_data, const address& event_address, const std::shared_ptr<addressable>& publisher)
    {
        Constrain(P, eventable);
        Val& subscriptions_opt = program.subscriptions_map.find(event_address);
        if (subscriptions_opt != std::end(program.subscriptions_map))
        {
            Val subscriptions_copy = *subscriptions_opt->second; 
            for (Val& subscription : subscriptions_copy)
            {
                Val cascade = publish_subscription<T, P>(*subscription, event_data, event_address, publisher, program);
                if (!cascade) break;
            }
        }
    }
}

#endif
