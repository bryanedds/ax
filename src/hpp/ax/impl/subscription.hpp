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
    public:

        CONSTRAINT(subscription_detail);

        subscription_detail() = delete;
        subscription_detail(const ax::handler<T, P>& handler) : handler(handler) { }
        subscription_detail(const subscription_detail& that) = delete;
        subscription_detail(subscription_detail&& that) = delete;
        subscription_detail& operator=(const subscription_detail& that) = delete;

        bool publish_subscription_detail(const event<T>& event, P& program)
        {
            return handler(event, program);
        }

    protected:

        using subscription_detail_T_P = subscription_detail<T, P>;
        ENABLE_CAST(subscription_detail_T_P, castable);

    private:

        const ax::handler<T, P> handler;
    };

    class subscription
    {
    public:

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

        template<typename T, typename P>
        bool publish_subscription(const T& event_data, const address& event_address, const std::shared_ptr<addressable>& publisher, P& program)
        {
            if (!subscriber_opt.expired())
            {
                VAL& subscriber = subscriber_opt.lock();
                VAL& event = ax::event<T>(event_data, event_address, subscriber, publisher);
                VAL& subscription_detail_opt = try_cast<ax::subscription_detail<T, P>>(*subscription_detail);
                if (subscription_detail_opt) return (*subscription_detail_opt)->publish_subscription_detail(event, program);
                return true;
            }
            return true;
        }

        const id_t id;
        const std::weak_ptr<addressable> subscriber_opt;
        const std::unique_ptr<castable> subscription_detail;
    };

    using subscription_list = std::vector<std::shared_ptr<subscription>>;

    using subscriptions_map = std::unordered_map<address, std::unique_ptr<subscription_list>>;

    using unsubscription_map = std::unordered_map<id_t, std::pair<address, std::weak_ptr<addressable>>>;
}

#endif
