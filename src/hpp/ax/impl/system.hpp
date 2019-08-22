#ifndef IMPL_AX_SYSTEM_HPP
#define IMPL_AX_SYSTEM_HPP

#include <cstddef>
#include <stdexcept>
#include <memory>
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
#include "eventable.hpp"

// NOTE: included in this file is an optimized entity-component-system prototype, an interesting seed for a fast C++
// game engine.

namespace ax
{
    class entity;

    struct component
    {
        CONSTRAINT(component);
        bool active;
    };

    struct entity_t : public ax::component
    {
        std::unordered_map<ax::name_t, ax::component*> components;
    };

    struct transform : public ax::component
    {
        // NOTE: of course, we'd instead use math library types here.
        float x_pos;
        float y_pos;
        float x_size;
        float y_size;
        float rotation;
    };

    class system : public castable
    {
    public:

        CONSTRAINT(system);
        virtual ax::component& add_component(const ax::address& address) = 0;
        virtual bool remove_component(const ax::address& address) = 0;
        virtual ax::component* try_get_component(const ax::address& address) = 0;

    protected:

        ENABLE_CAST(ax::system, ax::castable);
    };

    template<typename T>
    class system_t : public ax::system
    {
    public:

        CONSTRAINT(system_t);

        system_t(std::size_t capacity = 128)
        {
            CONSTRAIN(T, ax::component);
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
            }
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

    protected:

        ENABLE_CAST(ax::system_t<T>, ax::system);

    private:

        std::vector<T> components;
        std::unordered_map<ax::address, std::size_t> component_map;
        std::queue<std::size_t> free_list;
    };

    class world : public ax::eventable<world>
    {
    public:

        template<typename T>
        T* try_add_component(const ax::name_t& system_name, const ax::address& address, const T& component = T())
        {
            VAL& entities_iter = systems.find("entities");
            if (entities_iter != systems.end())
            {
                VAL& entities = ax::cast<ax::system_t<ax::entity_t>>(entities_iter->second);
                VAR* entity_opt = entities->try_get_component(address);
                if (entity_opt)
                {
                    VAL& system_iter = systems.find(system_name);
                    if (system_iter != systems.end())
                    {
                        VAL& system = ax::cast<ax::system_t<T>>(system_iter->second);
                        VAR& result = system->add_component(address, component);
                        entity_opt->components.insert_or_assign(system_name, &result);
                        return &result;
                    }
                }
            }
            return nullptr;
        }

        ax::transform* try_get_transform(const ax::address& address);
        ax::entity_t* try_get_entity(const ax::address& address);
        bool entity_exists(const ax::address& address);
        ax::component* try_add_component(const ax::name_t& system_name, const ax::address& address);
        bool try_remove_component(const ax::name_t& system_name, const ax::address& address);
        std::shared_ptr<ax::entity> create_entity(const ax::address& address);
        bool destroy_entity(const ax::address& address);

    private:

        ax::entity_t* try_add_entity(const ax::address& address);
        bool try_remove_entity(const ax::address& address);
        std::unordered_map<ax::name_t, std::shared_ptr<ax::system>> systems;
    };

    class entity : public addressable
    {
    public:

        entity(const ax::entity& entity) :
            entity(entity.get_address(), entity.world)
        { };

        entity(const ax::address& address, ax::world& world) :
            ax::addressable(address),
            world(world)
        { }

        template<typename T>
        const T* try_get_component(const ax::name_t& name) const { return world.try_get_component<T>(name, get_address()); }

        template<typename T>
        T* try_get_component(const ax::name_t& name) { return world.try_get_component<T>(name, get_address()); }

        template<typename T>
        const T& get_component(const ax::name_t& name) const { return *try_get_component<T>(name); }

        template<typename T>
        T& get_component(const ax::name_t& name) { return *try_get_component<T>(name); }

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
