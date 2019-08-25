#ifndef IMPL_AX_SUBSCRIPTION_HPP
#define IMPL_AX_SUBSCRIPTION_HPP

#include <cstddef>
#include <memory>
#include <functional>
#include <unordered_map>

#include "prelude.hpp"
#include "id.hpp"
#include "castable.hpp"
#include "address.hpp"
#include "addressable.hpp"
#include "event.hpp"
#include "vector.hpp"

namespace ax
{
    template<typename T, typename P>
    using handler = std::function<bool(const ax::event<T>&, P&)>;

    template<typename T, typename P>
    class subscription_detail : public ax::castable
    {
    public:

        CONSTRAINT(subscription_detail);

        subscription_detail() = delete;
        subscription_detail(const ax::handler<T, P>& handler) : handler(handler) { }
        subscription_detail(const subscription_detail& that) = delete;
        subscription_detail(subscription_detail&& that) = delete;
        subscription_detail& operator=(const subscription_detail& that) = delete;

        bool publish_subscription_detail(const ax::event<T>& event, P& program)
        {
            return handler(event, program);
        }

    protected:

        using subscription_detail_T_P = ax::subscription_detail<T, P>;
        ENABLE_CAST(subscription_detail_T_P, ax::castable);

    private:

        const ax::handler<T, P> handler;
    };

    class subscription
    {
    public:

        subscription() = delete;
        subscription(const ax::subscription&) = delete;
        subscription(ax::subscription&&) = delete;
        ax::subscription& operator=(const ax::subscription&) = delete;
        ax::subscription& operator=(ax::subscription&&) = delete;

        subscription(
            ax::id_t id,
            std::shared_ptr<ax::addressable> subscriber,
            std::unique_ptr<ax::castable> subscription_detail) :
            id(id),
            subscriber_opt(subscriber),
            subscription_detail(subscription_detail.release()) { }

        template<typename T, typename P>
        bool publish_subscription(const T& event_data, const ax::address& event_address, const std::shared_ptr<ax::addressable>& publisher, P& program)
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

        const ax::id_t id;
        const std::weak_ptr<ax::addressable> subscriber_opt;
        const std::unique_ptr<ax::castable> subscription_detail;
    };

    using subscription_list = ax::vector<std::shared_ptr<ax::subscription>>;

    using subscriptions_map = std::unordered_map<ax::address, std::unique_ptr<ax::subscription_list>>;

    using unsubscription_map = std::unordered_map<id_t, std::pair<address, std::weak_ptr<ax::addressable>>>;
}

#endif
