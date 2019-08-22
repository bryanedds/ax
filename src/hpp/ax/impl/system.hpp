#ifndef IMPL_AX_SYSTEM_HPP
#define IMPL_AX_SYSTEM_HPP

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>

#include "prelude.hpp"
#include "string.hpp"
#include "option.hpp"
#include "address.hpp"
#include "addressable.hpp"
#include "castable.hpp"

namespace ax
{
	class world;

	struct component
	{
		CONSTRAINT(component);
		bool active;
	};

	struct entity_t : public ax::component
	{
		std::unordered_map<std::string, ax::component*> components;
	};

	struct transform : public ax::component
	{
		float position;
		float orientation;
	};

	class system : public castable
	{
	protected:

		ENABLE_CAST(ax::system, ax::castable);

	public:

		CONSTRAINT(system);
		virtual ax::component& add_component(const ax::address& address) = 0;
		virtual bool remove_component(const ax::address& address) = 0;
		virtual ax::component* try_get_component(const ax::address& address) = 0;
	};

	template<typename T>
	class system_t : public ax::system
	{
	protected:

		ENABLE_CAST(ax::system_t<T>, ax::system);

	public:

		CONSTRAINT(system_t);

		system_t(std::size_t capacity = 128)
		{
			CONSTRAIN(T, ax::component); // can we verify the contraint at class scope instead?
			components.reserve(capacity);
			component_map.reserve(capacity);
		}

		T& add_component(const ax::address& address, const T& component)
		{
			if (free_list.empty())
			{
				components.push_back(component);
				VAR& component_found = components.back();
				component_found.active = false;
				return component_found;
			}
			else
			{
				VAL index = free_list.front();
				free_list.pop();
				component_map.insert_or_assign(address, index);
				VAR& component_found = components.at(index);
				component_found = component;
				component_found.active = true; // ensure active after assignment
				return component_found;
			}
		}

		T& add_component(const ax::address& address)
		{
			return ax::system_t<T>::add_component(address, T());
		}

		bool remove_component(const ax::address& address)
		{
			VAL index_iter = component_map.find(address);
			if (index_iter != component_map.end())
			{
				VAL index = index_iter->second;
				if (index == components.size() - 1)
				{
					components.erase(components.end() - 1);
				}
				else
				{
					free_list.push(index);
					VAR& component = components.at(index);
					component.active = false;
				}
				component_map.erase(address);
			}
		}

		T* try_get_component(const ax::address& address)
		{
			VAL index_iter = component_map.find(address);
			if (index_iter != component_map.end())
			{
				VAL index = index_iter->second;
				return &components.at(index);
			}
			return nullptr;
		}

	private:

		std::vector<T> components;
		std::unordered_map<ax::address, std::size_t> component_map;
		std::queue<std::size_t> free_list;
	};

	class entity : public addressable
	{
	private:

		ax::world& world;

	public:

		entity(const ax::address& address, ax::world& world) :
			ax::addressable(address),
			world(world)
		{ }

		ax::transform& get_transform() const { return *world.try_get_transform(get_address()); }
		ax::transform& set_transform(const ax::transform& transform) { return get_transform() = transform; }

		float get_position() const { return get_transform().position; }
		float set_position(float value) { return get_transform().position = value; }

		float get_orientation() const { return get_transform().orientation; }
		float set_orientation(float value) { return get_transform().orientation = value; }

		bool exists() const { return world.entity_exists(get_address()); }
	};

	class world
	{
	public:

		ax::transform* try_get_transform(const ax::address& address)
		{
			VAR& transforms_iter = systems.find("transforms");
			if (transforms_iter != systems.end())
			{
				VAR& transforms = ax::cast<ax::system_t<ax::transform>>(transforms_iter->second);
				return transforms->try_get_component(address);
			}
			return nullptr;
		}

		ax::entity_t* try_get_entity(const ax::address& address)
		{
			VAR& entities_iter = systems.find("entities");
			if (entities_iter != systems.end())
			{
				VAR& entities = ax::cast<ax::system_t<ax::entity_t>>(entities_iter->second);
			  	return entities->try_get_component(address);
			}
			return nullptr;
		}

		bool entity_exists(const ax::address& address)
		{
			return try_get_entity(address);
		}

		template<typename T>
		T* try_add_component(const std::string& system_name, const ax::address& address, const T& component = T())
		{
			VAR& entities_iter = systems.find("entities");
			if (entities_iter != systems.end())
			{
				VAR& entities = ax::cast<ax::system_t<ax::entity_t>>(entities_iter->second);
				VAR* entity_opt = entities->try_get_component(address);
				if (entity_opt)
				{
					VAR& system_iter = systems.find(system_name);
					if (system_iter != systems.end())
					{
						VAR& system = ax::cast<ax::system_t<T>>(system_iter->second);
						VAR& result = system->add_component(address, component);
						entity_opt->components.insert_or_assign(system_name, &result);
						return &result;
					}
				}
			}
			return nullptr;
		}

		ax::component* try_add_component(const std::string& system_name, const ax::address& address)
		{
			VAR& entities_iter = systems.find("entities");
			if (entities_iter != systems.end())
			{
				VAR& entities = ax::cast<ax::system_t<ax::entity_t>>(entities_iter->second);
				VAR* entity_opt = entities->try_get_component(address);
				if (entity_opt)
				{
					VAR& system_iter = systems.find(system_name);
					if (system_iter != systems.end())
					{
						VAR& system = system_iter->second;
						VAR& result = system->add_component(address);
						entity_opt->components.insert_or_assign(system_name, &result);
						return &result;
					}
				}
			}
			return nullptr;
		}

		bool try_remove_component(const std::string& system_name, const ax::address& address)
		{
			VAR& entities_iter = systems.find("entities");
			if (entities_iter != systems.end())
			{
				VAR& entities = ax::cast<ax::system_t<ax::entity_t>>(entities_iter->second);
				VAR* entity_opt = entities->try_get_component(address);
				if (entity_opt)
				{
					VAR& system_iter = systems.find(system_name);
					if (system_iter != systems.end())
					{
						VAR& system = system_iter->second;
						VAR result = system->remove_component(address);
						entity_opt->components.erase(system_name);
						return result;
					}
				}
			}
			return nullptr;
		}

		ax::entity create_entity(const ax::address& address)
		{
			VAR& entities_iter = systems.find("entities");
			if (entities_iter != systems.end())
			{
				VAR& entities = ax::cast<ax::system_t<ax::entity_t>>(entities_iter->second);
				VAR* entity_opt = entities->try_get_component(address);
				if (entity_opt)
				{
					try_add_entity(address);
					try_add_component<ax::transform>("transform", address);
				}
			}
			return ax::entity(address, *this);
		}

		bool destroy_entity(const ax::address& address)
		{
			VAR& entities_iter = systems.find("entities");
			if (entities_iter != systems.end())
			{
				VAR& entities = ax::cast<ax::system_t<ax::entity_t>>(entities_iter->second);
				VAR* entity_opt = entities->try_get_component(address);
				if (entity_opt)
				{
					try_remove_component("transform", address);
					return try_remove_entity(address);
				}
			}
			return false;
		}

	private:

		ax::entity_t* try_add_entity(const ax::address& address)
		{
			VAR& entities_iter = systems.find("entities");
			if (entities_iter != systems.end())
			{
				VAR& entities = ax::cast<ax::system_t<ax::entity_t>>(entities_iter->second);
				VAR* entity_opt = entities->try_get_component(address);
				if (!entity_opt) return &entities->add_component(address);
			}
			return nullptr;
		}

		bool try_remove_entity(const ax::address& address)
		{
			VAR& entities_iter = systems.find("entities");
			if (entities_iter != systems.end())
			{
				VAR& entities = ax::cast<ax::system_t<ax::entity_t>>(entities_iter->second);
				VAR* entity_opt = entities->try_get_component(address);
				if (entity_opt) return entities->remove_component(address);
			}
			return false;
		}

		std::unordered_map<std::string, std::shared_ptr<ax::system>> systems;
	};
}

#endif
