#ifndef AX_PROPERTIED_HPP
#define AX_PROPERTIED_HPP

#include <cstddef>

#include "prelude.hpp"
#include "name.hpp"
#include "reflectable.hpp"
#include "property.hpp"

namespace ax
{
    class propertied : public ax::reflectable
    {
    public:

        CONSTRAINT(propertied);
        propertied() = default;

        template<typename T>
        const T& get(const ax::name& name) const
        {
            return properties.get_property<T>(name).get_value();
        }

        template<typename T>
        T& get(const ax::name& name)
        {
            return properties.get_property<T>(name).get_value();
        }

        template<typename T>
        T& set(const ax::name& name, const T& value)
        {
            return properties.get_property<T>(name).set_value(value);
        }

        template<typename T>
        T& set(const ax::name& name, T&& value)
        {
            return properties.get_property<T>(name).set_value(value);
        }

        template<typename T>
        void attach(const ax::name& name, const T& value)
        {
            properties.attach_property<T>(name, value);
        }

        template<typename T>
        void attach(const ax::name& name, T&& value)
        {
            properties.attach_property<T>(name, value);
        }

    protected:

        ENABLE_CAST(ax::propertied, ax::reflectable);

    private:

        ax::property_map properties;
    };
}

#endif
