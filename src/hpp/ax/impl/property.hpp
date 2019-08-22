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

		template<typename T>
		const T& get_value() const
		{
			return **this;
		}

		template<typename T>
		T& get_value()
		{
			return **this;
		}

		template<typename T>
		T& set_value(const T& value)
		{
			return *this = value;
		}

		template<typename T>
		T& set_value(T&& value)
		{
			return *this = value;
		}

    protected:

        ENABLE_CAST(property<T>, castable);

	private:

		T data;
    };

    // TODO: type descriptor
    class property_map : public std::unordered_map<name_t, std::unique_ptr<castable>>
    {
    public:

        CONSTRAINT(property_map);
        using std::unordered_map<name_t, std::unique_ptr<castable>>::unordered_map;

		template<typename T>
		const property<T>& get_property(const name_t& name) const
		{
			VAL& property_opt = find(name);
			if (property_opt != end()) return cast<property<T>>(*property_opt->second);
			throw std::logic_error("No such property '"_s + get_name_str(name) + "'.");
		}

		template<typename T>
		property<T>& get_property(const name_t& name)
		{
			VAL& property_opt = find(name);
			if (property_opt != end()) return cast<property<T>>(*property_opt->second);
			throw std::logic_error("No such property '"_s + name.get_name_str() + "'.");
		}

		template<typename T>
		void attach_property(const name_t& name, const T& value)
		{
			insert(std::make_pair(name, std::make_unique<property<T>>(value)));
		}

		template<typename T>
		void attach_property(const name_t& name, T&& value)
		{
			insert(name, std::make_unique<property<T>>(value));
		}
    };
}

#endif
