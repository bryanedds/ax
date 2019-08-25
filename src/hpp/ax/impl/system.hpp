#ifndef IMPL_AX_SYSTEM_HPP
#define IMPL_AX_SYSTEM_HPP

#include <cstddef>
#include <stdexcept>
#include <memory>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <queue>

#include "prelude.hpp"
#include "string.hpp"
#include "vector.hpp"
#include "option.hpp"
#include "address.hpp"
#include "addressable.hpp"
#include "castable.hpp"
#include "eventable.hpp"

// NOTE: included in this file is an optimized entity-component-system prototype, an interesting seed for a fast C++
// game engine. This prototype is limited in functionality by disallowing more than one of any type of component per
// entity. This may not be an acceptable limitation in practical usage.

namespace ax
{
    class entity;
    class system;
    using system_ptr = std::shared_ptr<ax::system>;
    class world;

    // A component in an entity-component-system.
    struct component
    {
        CONSTRAINT(component);
        bool active;
    };

    // A component that includes the address of the containing entity so that related components can be found.
    struct composite_component : public ax::component
    {
        CONSTRAINT(composite_component);
        ax::address address;
    };

    // The common data of an entity stored as a component.
    struct entity_component : public ax::component
    {
        std::unordered_map<ax::name_t, ax::component*> components;
    };

    // A transform component.
    // NOTE: of course, we'd instead use math library types in here.
    struct transform : public ax::component
    {
        float x_pos;
        float y_pos;
        float rotation;
    };

    // A system in an entity-component-system.
    class system : public ax::castable
    {
    public:

        CONSTRAINT(system);
        system(ax::world& world) : world(world) { }
        virtual ~system() = default;

        virtual ax::component& add_component(const ax::address& address) = 0;
        virtual bool remove_component(const ax::address& address) = 0;
        virtual ax::component* try_get_component(const ax::address& address) = 0;
        virtual void update(int mode = 0) = 0;

    protected:

        ENABLE_CAST(ax::system, ax::castable);
        ax::world& world;
    };

    template<typename T>
    class system_t : public ax::system
    {
    public:

        CONSTRAINT(system_t);

        system_t(std::size_t capacity = 128)
        {
            CONSTRAIN(T, component);
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
                std::insert_or_assign(component_map, address, index);
                VAR& component_found = components.at(index);
                component_found = component;
                component_found.active = true; // ensure active after assignment
                return component_found;
            }
        }

        T& add_component(const ax::address& address) override
        {
            return ax::system_t<T>::add_component(address, T());
        }

        bool remove_component(const ax::address& address) override
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
                return true;
            }
            return false;
        }

        T* try_get_component(const ax::address& address) override
        {
            VAL index_iter = component_map.find(address);
            if (index_iter != component_map.end())
            {
                VAL index = index_iter->second;
                return &components.at(index);
            }
            return nullptr;
        }

        void update(int mode) override
        {
            for (VAR& component : components)
            {
                update_component(component, mode);
            }
        }

    protected:

        ENABLE_CAST(ax::system_t<T>, ax::system);
        virtual void update_component(T& component, int mode) = 0;

        ax::vector<T> components;
        std::unordered_map<ax::address, std::size_t> component_map;
        std::queue<std::size_t> free_list;
    };

    // The world that contains the entity-component-system, event system, and other mixins.
    // Uses function members because the type is not meant to be inherited.
    class world : public ax::eventable<ax::world>
    {
    public:

        world(
            std::function<void(ax::world& world)> initialize_systems_impl,
            std::function<void(ax::world& world)> update_systems_impl,
            std::function<void(ax::world& world)> clean_up_systems_impl);

        template<typename T>
        T* try_add_component(const ax::name_t& system_name, const ax::address& address, const T& component = T())
        {
            VAL& entities_iter = systems.find("entities");
            if (entities_iter != systems.end())
            {
                VAL& entities = ax::cast<ax::system_t<ax::entity_component>>(entities_iter->second);
                VAR* entity_opt = entities->try_get_component(address);
                if (entity_opt)
                {
                    VAL& system_iter = systems.find(system_name);
                    if (system_iter != systems.end())
                    {
                        VAL& system = ax::cast<ax::system_t<T>>(system_iter->second);
                        VAR& result = system->add_component(address, component);
                        std::insert_or_assign(entity_opt->components, system_name, &result);
                        return &result;
                    }
                }
            }
            return nullptr;
        }

        template<typename T>
        T* try_get_component(const ax::name_t& system_name, const ax::address& address)
        {
            VAL& entities_iter = systems.find("entities");
            if (entities_iter != systems.end())
            {
                VAL& entities = ax::cast<ax::system_t<ax::entity_component>>(entities_iter->second);
                VAR* entity_opt = entities->try_get_component(address);
                if (entity_opt)
                {
                    VAL& system_iter = systems.find(system_name);
                    if (system_iter != systems.end())
                    {
                        VAL& system = ax::cast<ax::system_t<T>>(system_iter->second);
                        return system->try_get_component(address);
                    }
                }
            }
            return nullptr;
        }

        ax::transform* try_get_transform(const ax::address& address);
        ax::entity_component* try_get_entity(const ax::address& address);
        bool entity_exists(const ax::address& address);
        ax::component* try_add_component(const ax::name_t& system_name, const ax::address& address);
        bool try_remove_component(const ax::name_t& system_name, const ax::address& address);
        ax::entity create_entity(const ax::address& address);
        bool destroy_entity(const ax::address& address);
        ax::system_ptr try_add_system(const ax::name_t& name, ax::system_ptr system);
        bool remove_system(const ax::name_t& name);
        void initialize_systems();
        void update_systems();
        void clean_up_systems();

    private:

        ax::entity_component* try_add_entity(const ax::address& address);
        bool try_remove_entity(const ax::address& address);
        std::unordered_map<ax::name_t, std::shared_ptr<ax::system>> systems;
        std::function<void(ax::world& world)> initialize_systems_impl;
        std::function<void(ax::world& world)> update_systems_impl;
        std::function<void(ax::world& world)> clean_up_systems_impl;
    };

    // A handle to an entity in an entity-component-system.
    class entity : public addressable
    {
    public:

        entity(const ax::entity& other) = default;
        entity(ax::entity&& other) = default;
        ax::entity& operator=(const ax::entity& other) = default;
        ax::entity& operator=(ax::entity&& other) = default;
        entity(const ax::address& address, ax::world& world) : ax::addressable(address), world(world) { }

        template<typename T>
        const T* try_get_component(const ax::name_t& name) const { return world.try_get_component<T>(name, get_address()); }

        template<typename T>
        T* try_get_component(const ax::name_t& name) { return world.try_get_component<T>(name, get_address()); }

        template<typename T>
        const T& get_component(const ax::name_t& name) const { return *try_get_component<T>(name); }

        template<typename T>
        T& get_component(const ax::name_t& name)
        {
            VAR* component_opt = *try_get_component<T>(name);
            if (component_opt) return *component_opt;
            throw std::runtime_error("No component "_s + name.to_string() + " found at address " + get_address().to_string());
        }

        ax::transform& get_transform() const { return *world.try_get_transform(get_address()); }
        ax::transform& set_transform(const ax::transform& transform) { return get_transform() = transform; }
        float get_x_pos() const { return get_transform().x_pos; }
        float set_x_pos(float value) { return get_transform().x_pos = value; }
        float get_y_pos() const { return get_transform().y_pos; }
        float set_y_pos(float value) { return get_transform().y_pos = value; }
        float get_rotation() const { return get_transform().rotation; }
        float set_rotation(float value) { return get_transform().rotation = value; }
        bool exists() const { return world.entity_exists(get_address()); }

    private:

        ax::world& world;
    };
}

#endif
