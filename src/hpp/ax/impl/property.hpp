#ifndef IMPL_AX_PROPERTY_HPP
#define IMPL_AX_PROPERTY_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <unordered_map>

#include "prelude.hpp"
#include "name.hpp"
#include "castable.hpp"

namespace ax
{
    template<typename T>
    class property final : public ax::castable
    {
    public:

        CONSTRAINT(property);
        template<typename A> using reify = ax::property<A>;

        property() = default;
        property(const property&) = default;
        property(property&&) = default;
        property& operator=(const property&) = default;
        property& operator=(property&&) = default;

        explicit property(const T& value) : data(value) { }
        T& operator=(const T& value) { return data = value; }
        T& operator=(T&& value) { return data = value; }
        const T& operator*() const { return data; }
        T& operator*() { return data; }

        const T& get_value() const { return **this; }
        T& get_value() { return **this; }
        T& set_value(const T& value) { return *this = value; }
        T& set_value(T&& value) { return *this = value; }

    protected:

        ENABLE_CAST(ax::property<T>, ax::castable);

    private:

        T data;
    };

    // TODO: type descriptor
    struct property_map final : public std::unordered_map<ax::name, std::unique_ptr<ax::castable>>
    {
    public:

        CONSTRAINT(property_map);
        using std::unordered_map<ax::name, std::unique_ptr<ax::castable>>::unordered_map;

        template<typename T>
        const ax::property<T>& get_property(const ax::name& name) const
        {
            VAL& property_opt = find(name);
            if (property_opt != end()) return ax::cast<ax::property<T>>(*property_opt->second);
            throw std::logic_error("No such property '"_s + std::to_string(name) + "'.");
        }

        template<typename T>
        ax::property<T>& get_property(const ax::name& name)
        {
            VAL& property_opt = find(name);
            if (property_opt != end()) return ax::cast<ax::property<T>>(*property_opt->second);
            throw std::logic_error("No such property '"_s + std::to_string(name) + "'.");
        }

        template<typename T>
        void attach_property(const ax::name& name, const T& value)
        {
            insert(std::make_pair(name, std::make_unique<ax::property<T>>(value)));
        }

        template<typename T>
        void attach_property(const ax::name& name, T&& value)
        {
            insert(name, std::make_unique<ax::property<T>>(value));
        }
    };
}

#endif
