#ifndef IMPL_AX_PROPERTIED_HPP
#define IMPL_AX_PROPERTIED_HPP

#include <cstddef>

#include "prelude.hpp"
#include "reflectable.hpp"
#include "property.hpp"

namespace ax
{
    class propertied : public reflectable
    {
    private:

        property_map properties;

    protected:

        enable_cast(propertied, reflectable);

        template<typename T>
        friend const T& get(const propertied& propertied, const name_t& name);

        template<typename T>
        friend T& get(propertied& propertied, const name_t& name);

        template<typename T>
        friend T& set(propertied& propertied, const name_t& name, const T& value);

        template<typename T>
        friend T& set(propertied& propertied, const name_t& name, T&& value);

        template<typename T>
        friend void attach(propertied& propertied, const name_t& name, const T& value);

        template<typename T>
        friend void attach(propertied& propertied, const name_t& name, T&& value);

    public:

        propertied() = default;
    };

    template<typename T>
    const T& get(const propertied& propertied, const name_t& name)
    {
        return get_value<T>(get_property<T>(propertied.properties, name));
    }

    template<typename T>
    T& get(propertied& propertied, const name_t& name)
    {
        return get_value<T>(get_property<T>(propertied.properties, name));
    }

    template<typename T>
    T& set(propertied& propertied, const name_t& name, const T& value)
    {
        return set_value<T>(get_property<T>(propertied.properties, name), value);
    }

    template<typename T>
    T& set(propertied& propertied, const name_t& name, T&& value)
    {
        return set_value<T>(get_property<T>(propertied.properties, name), value);
    }

    template<typename T>
    void attach(propertied& propertied, const name_t& name, const T& value)
    {
        attach_property<T>(propertied.properties, name, value);
    }

    template<typename T>
    void attach(propertied& propertied, const name_t& name, T&& value)
    {
        attach_property<T>(propertied.properties, name, value);
    }
}

#endif
