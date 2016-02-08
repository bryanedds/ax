#ifndef IMPL_AX_SUBSCRIPTION_HPP
#define IMPL_AX_SUBSCRIPTION_HPP

#include <cstddef>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>

#include "prelude.hpp"
#include "id.hpp"
#include "castable.hpp"
#include "address.hpp"
#include "addressable.hpp"
#include "event.hpp"

namespace ax
{
    template<typename T, typename P>
    using handler = std::function<bool(const event<T>&, P&)>;

    template<typename T, typename P>
    class subscription_detail : public castable
    {
    private:

        const ax::handler<T, P> handler;

    protected:

        using subscription_detail_T_P = subscription_detail<T, P>;
        enable_cast(subscription_detail_T_P, castable);

        template<typename A, typename B>
        friend bool publish_subscription_detail(const subscription_detail<A, B>& subscription_detail, const event<A>& event, B& program);

    public:

        constraint(subscription_detail);

        subscription_detail() = delete;
        subscription_detail(const ax::handler<T, P>& handler) : handler(handler) { }
        subscription_detail(const subscription_detail& that) = delete;
        subscription_detail(subscription_detail&& that) = delete;
        subscription_detail& operator=(const subscription_detail& that) = delete;
    };

    template<typename T, typename P>
    bool publish_subscription_detail(const subscription_detail<T, P>& subscription_detail, const event<T>& event, P& program)
    {
        return subscription_detail.handler(event, program);
    }

    class subscription
    {
    protected:

        template<typename T, typename P>
        friend bool publish_subscription(const subscription& subscription, const T& event_data, const address& event_address, const std::shared_ptr<addressable>& publisher, P& program);

    public:

        const id_t id;
        const std::weak_ptr<addressable> subscriber_opt;
        const std::unique_ptr<castable> subscription_detail;

        subscription() = delete;
        subscription(const subscription&) = delete;
        subscription(subscription&&) = delete;
        subscription& operator=(const subscription&) = delete;
        subscription& operator=(subscription&&) = delete;

        subscription(
            id_t id,
            std::shared_ptr<addressable> subscriber,
            std::unique_ptr<castable> subscription_detail) :
            id(id),
            subscriber_opt(subscriber),
            subscription_detail(subscription_detail.release()) { }
    };

    template<typename T, typename P>
    bool publish_subscription(const subscription& subscription, const T& event_data, const address& event_address, const std::shared_ptr<addressable>& publisher, P& program)
    {
        if (!subscription.subscriber_opt.expired())
        {
            Val& subscriber = subscription.subscriber_opt.lock();
            Val& event = ax::event<T>(event_data, event_address, subscriber, publisher);
            Val& subscription_detail_opt = try_cast<subscription_detail<T, P>>(*subscription.subscription_detail);
            if (subscription_detail_opt) return publish_subscription_detail(**subscription_detail_opt, event, program);
            return true;
        }
        return true;
    }

    using subscription_list = std::vector<std::shared_ptr<subscription>>;

    using subscriptions_map = std::unordered_map<address, std::unique_ptr<subscription_list>>;

    using unsubscription_map = std::unordered_map<id_t, std::pair<address, std::weak_ptr<addressable>>>;
}

#endif