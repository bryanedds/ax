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
    class property : public castable
    {
    private:

        T data;

    protected:

        ENABLE_CAST(property<T>, castable);

    public:

        CONSTRAINT(property);
        template<typename A>
        using reify = property<A>;

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
    };

    template<typename T>
    const T& get_value(const property<T>& property)
    {
        return *property;
    }

    template<typename T>
    T& get_value(property<T>& property)
    {
        return *property;
    }

    template<typename T>
    T& set_value(property<T>& property, const T& value)
    {
        return property = value;
    }

    template<typename T>
    T& set_value(property<T>& property, T&& value)
    {
        return property = value;
    }

    // TODO: type descriptor
    class property_map : public std::unordered_map<name_t, std::unique_ptr<castable>>
    {
    public:
        CONSTRAINT(property_map);
        using std::unordered_map<name_t, std::unique_ptr<castable>>::unordered_map;
    };

    template<typename T>
    const property<T>& get_property(const property_map& properties, const name_t& name)
    {
        VAL& property_opt = properties.find(name);
        if (property_opt != properties.end()) return cast<property<T>>(*property_opt->second);
        throw std::logic_error("No such property '"_s + get_name_str(name) + "'.");
    }

    template<typename T>
    property<T>& get_property(property_map& properties, const name_t& name)
    {
        VAL& property_opt = properties.find(name);
        if (property_opt != properties.end()) return cast<property<T>>(*property_opt->second);
        throw std::logic_error("No such property '"_s + get_name_str(name) + "'.");
    }

    template<typename T>
    void attach_property(property_map& properties, const name_t& name, const T& value)
    {
        properties.insert(std::make_pair(name, std::make_unique<property<T>>(value)));
    }

    template<typename T>
    void attach_property(property_map& properties, const name_t& name, T&& value)
    {
        properties.insert(name, std::make_unique<property<T>>(value));
    }
}

#endif
