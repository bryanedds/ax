#ifndef IMPL_AX_SYSTEM_HPP
#define IMPL_AX_SYSTEM_HPP

#include <cstddef>
#include <stdexcept>
#include <memory>
#include <functional>
#include <array>
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
// game engine.

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
        std::unordered_map<ax::name, ax::component*> components;
    };

    // A component that store multiple of the same type of component.
    template<typename T, typename A, std::size_t N>
    struct multi_component : public ax::component
    {
        CONSTRAINT(multi_component);
        constexpr void static_check() { CONSTRAIN(T, component); }
        ax::vector<T, A, N> components;
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
        using component_t = T;
        template<typename T> using reify = ax::system_t<T>;

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
                free_list.push(index);
                VAR& component = components.at(index);
                component.active = false;
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

        virtual void update_component(T& component, int mode) = 0;

    protected:

        ENABLE_CAST(ax::system_t<T>, ax::system);

        ax::vector<T> components;
        std::unordered_map<ax::address, std::size_t> component_map;
        std::queue<std::size_t> free_list;
    };

    template<typename S, typename A, std::size_t N>
    class multi_system final : public ax::system_t<ax::multi_component<typename S::component_t, A, N>>
    {
    public:

        CONSTRAINT(multi_system);
        using component_t = typename S::component_t;
        using multi_component_t = typename ax::multi_component<component_t, A, N>;
        template<typename S, typename A, std::size_t N> using reify = ax::multi_system<S, A, N>;

        void update_component(multi_component_t& multicomponent, int mode) override
        {
            CONSTRAIN(S, ax::system_t<component_t>);
            for (VAR& component : multi_component.components)
            {
                system.update_component(component, mode);
            }
        }

    protected:

        using multi_system_s_a_n = ax::multi_system<S, A, N>;
        using system_t_c = ax::system_t<component_t>;
        ENABLE_CAST(multi_system_s_a_n, system_t_c);

        S& system;
    };

    // The world that contains the entity-component-system, event system, and other mixins.
    // Uses function members because the type is not meant to be inherited.
    class world final : public ax::eventable<ax::world>
    {
    public:

        world(
            std::function<void(ax::world& world)> initialize_systems_impl,
            std::function<void(ax::world& world)> update_systems_impl,
            std::function<void(ax::world& world)> clean_up_systems_impl);

        template<typename T>
        T* try_add_component(const ax::name& system_name, const ax::address& address, const T& component = T())
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
        T* try_get_component(const ax::name& system_name, const ax::address& address)
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
        ax::component* try_add_component(const ax::name& system_name, const ax::address& address);
        bool try_remove_component(const ax::name& system_name, const ax::address& address);
        ax::entity create_entity(const ax::address& address);
        bool destroy_entity(const ax::address& address);
        ax::system_ptr try_add_system(const ax::name& name, ax::system_ptr system);
        bool remove_system(const ax::name& name);
        void initialize_systems();
        void update_systems();
        void clean_up_systems();

    private:

        ax::entity_component* try_add_entity(const ax::address& address);
        bool try_remove_entity(const ax::address& address);
        std::unordered_map<ax::name, std::shared_ptr<ax::system>> systems;
        std::function<void(ax::world& world)> initialize_systems_impl;
        std::function<void(ax::world& world)> update_systems_impl;
        std::function<void(ax::world& world)> clean_up_systems_impl;
    };

    // A handle to an entity in an entity-component-system.
    class entity final : public ax::addressable
    {
    public:

        entity(const ax::entity& other) = default;
        entity(ax::entity&& other) = default;
        ax::entity& operator=(const ax::entity& other) = default;
        ax::entity& operator=(ax::entity&& other) = default;
        entity(const ax::address& address, ax::world& world) : ax::addressable(address), world(world) { }

        template<typename T>
        const T* try_get_component(const ax::name& name) const { return world.try_get_component<T>(name, get_address()); }

        template<typename T>
        T* try_get_component(const ax::name& name) { return world.try_get_component<T>(name, get_address()); }

        template<typename T>
        const T& get_component(const ax::name& name) const { return *try_get_component<T>(name); }

        template<typename T>
        T& get_component(const ax::name& name)
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
