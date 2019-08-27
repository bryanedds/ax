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
#include "property.hpp"

namespace ax
{
    class entity;
    class entity_behavior;
    class system;
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

    // A component that store multiple of the same type of component.
    template<typename T, typename Allocator, std::size_t N>
    struct multi_component : public ax::component
    {
        CONSTRAINT(multi_component);
        template<typename A, typename B, std::size_t C> using reify = ax::multi_component<A, B, C>;
        constexpr void static_check() { CONSTRAIN(T, component); }
        ax::vector<T, Allocator, N> components;
    };

    // The common data of an entity stored as a component.
    struct entity_component : public ax::component
    {
        std::unordered_map<ax::name, ax::component*> components;
    };

    // An entity behavior component.
    struct entity_behavior_component : public ax::component
    {
        std::shared_ptr<ax::entity_behavior> behavior;
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
        virtual void update(int mode = 0) = 0;

    protected:

        ENABLE_CAST(ax::system, ax::castable);
        ax::world& world;
    };

    // A singleton system.
    template<typename T>
    class system_s : public ax::system
    {
    public:

        CONSTRAINT(system_s);
        using component_t = T;
        template<typename A> using reify = ax::system_s<A>;

        system_s(ax::world& world) : system(world) { }
        const T& get_component() const { return component; }
        T& get_component() { return component; }

    protected:

        ENABLE_CAST(ax::system_s<T>, ax::system);
        T component;
    };

    // A non-singleton system.
    class system_n : public ax::system
    {
    public:

        CONSTRAINT(system_n);
        system_n(ax::world& world) : system(world) { }
        virtual ax::component& add_component(const ax::address& address) = 0;
        virtual bool remove_component(const ax::address& address) = 0;
        virtual ax::component* try_get_component(const ax::address& address) = 0;

    protected:

        ENABLE_CAST(ax::system_n, ax::system);
    };

    // A generic non-singleton system.
    template<typename T>
    class system_t : public ax::system_n
    {
    public:

        CONSTRAINT(system_t);
        using component_t = T;
        template<typename A> using reify = ax::system_t<A>;

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
                component_map[address] = index;
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

        ENABLE_CAST(ax::system_t<T>, ax::system_n);

        ax::vector<T> components;
        std::unordered_map<ax::address, std::size_t> component_map;
        std::queue<std::size_t> free_list;
    };

    // A multi-component system.
    template<typename S, typename Allocator, std::size_t N>
    class multi_system_t final : public ax::system_t<ax::multi_component<typename S::component_t, Allocator, N>>
    {
    public:

        CONSTRAINT(multi_system_t);
        using component_t = typename S::component_t;
        using multi_component_t = typename ax::multi_component<component_t, Allocator, N>;
        using system_t_t = ax::system_t<component_t>;
        template<typename A, typename B, std::size_t C> using reify = ax::multi_system_t<A, B, C>;

        multi_system_t(S& system) : system(system) { }

        void update_component(multi_component_t& multi_component, int mode) override
        {
            CONSTRAIN(S, system_t_t);
            for (VAR& component : multi_component.components)
            {
                system.update_component(component, mode);
            }
        }

    protected:

        using multi_system_s_a_n = ax::multi_system_t<S, Allocator, N>;
        ENABLE_CAST(multi_system_s_a_n, system_t_t);
        S& system;
    };

    // The world that contains the entity-component-system, event system, and other mixins.
    class world final : public ax::eventable<ax::world>
    {
    public:

        world(
            std::function<void(ax::world& world)> initialize_systems_impl,
            std::function<void(ax::world& world)> update_systems_impl,
            std::function<void(ax::world& world)> clean_up_systems_impl);

        ~world();

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
                        VAL& system_opt = ax::try_cast<ax::system_t<T>>(system_iter->second);
                        if (system_opt)
                        {
                            VAR& result = system_opt->add_component(address, component);
                            entity_opt->components[system_name] = &result;
                            return &result;
                        }
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
                        VAL& system_opt = ax::try_cast<ax::system_t<T>>(system_iter->second);
                        if (system_opt) return system_opt->try_get_component(address);
                    }
                }
            }
            return nullptr;
        }

        template<typename T>
        T* try_get_component(const ax::name& system_name)
        {
            VAL& system_iter = systems.find(system_name);
            if (system_iter != systems.end())
            {
                VAL& system_opt = ax::try_cast<ax::system_s<T>>(system_iter->second);
                if (system_opt) return &system_opt->get_component();
            }
            return nullptr;
        }

        template <typename Behavior>
        ax::entity create_entity_with_behavior(const ax::address& address)
        {
            CONSTRAIN(Behavior, ax::entity_behavior);
            VAL entity = create_entity(address);
            VAL behavior_ptr = std::make_shared<Behavior>(entity, this*);
            VAL behavior_component = entity_behavior_component{ behavior };
            try_add_component<ax::entity_behavior_component>("entity_behaviors", address, behavior_component);
            return entity;
        }

        ax::transform* try_get_transform(const ax::address& address);
        ax::entity_component* try_get_entity(const ax::address& address);
        ax::entity_behavior_component* try_get_entity_behavior(const ax::address& address);
        bool entity_exists(const ax::address& address);
        ax::component* try_add_component(const ax::name& system_name, const ax::address& address);
        bool try_remove_component(const ax::name& system_name, const ax::address& address);
        ax::entity create_entity(const ax::address& address);
        bool destroy_entity(const ax::address& address);
        std::shared_ptr<ax::system> try_add_system(const ax::name& name, std::shared_ptr<ax::system> system);
        bool remove_system(const ax::name& name);
        void update_systems();

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
        const T* try_get_component(const ax::name& name) const
        {
            return world.try_get_component<T>(name, get_address());
        }

        template<typename T>
        T* try_get_component(const ax::name& name)
        {
            return world.try_get_component<T>(name, get_address());
        }

        template<typename T>
        const T& get_component(const ax::name& name) const
        {
            VAR* component_opt = *try_get_component<T>(name);
            if (component_opt) return *component_opt;
            throw std::runtime_error("No component "_s + name.to_string() + " found at address " + get_address().to_string());
        }

        template<typename T>
        T& get_component(const ax::name& name)
        {
            VAR* component_opt = *try_get_component<T>(name);
            if (component_opt) return *component_opt;
            throw std::runtime_error("No component "_s + name.to_string() + " found at address " + get_address().to_string());
        }

        template<typename T>
        const T& get(const ax::name& name) const
        {
            return get_behavior_properties().get_property<T>(name).get_value();
        }

        template<typename T>
        T& get(const ax::name& name)
        {
            return get_behavior_properties().get_property<T>(name).get_value();
        }

        template<typename T>
        T& set(const ax::name& name, const T& value)
        {
            return get_behavior_properties().get_property<T>(name).set_value(value);
        }

        template<typename T>
        T& set(const ax::name& name, T&& value)
        {
            return get_behavior_properties().get_property<T>(name).set_value(value);
        }

        template<typename T>
        void attach(const ax::name& name, const T& value)
        {
            get_behavior_properties().attach_property<T>(name, value);
        }

        template<typename T>
        void attach(const ax::name& name, T&& value)
        {
            get_behavior_properties().attach_property<T>(name, value);
        }

        const ax::property_map* try_get_behavior_properties() const;
        ax::property_map* try_get_behavior_properties();
        const ax::property_map& get_behavior_properties() const;
        ax::property_map& get_behavior_properties();
        std::shared_ptr<const ax::entity_behavior> try_get_behavior() const;
        std::shared_ptr<ax::entity_behavior> try_get_behavior();
        const ax::entity_behavior& get_behavior() const;
        ax::entity_behavior& get_behavior();

        const ax::transform& get_transform() const;
        ax::transform& get_transform();
        ax::transform& set_transform(const ax::transform& transform);
        float get_x_pos() const { return get_transform().x_pos; }
        float set_x_pos(float value) { return get_transform().x_pos = value; }
        float get_y_pos() const { return get_transform().y_pos; }
        float set_y_pos(float value) { return get_transform().y_pos = value; }
        float get_rotation() const { return get_transform().rotation; }
        float set_rotation(float value) { return get_transform().rotation = value; }
        bool exists() const;

    private:

        ax::world& world;
    };

    // Allows high-level definition of entity behavior.
    class entity_behavior : public ax::castable
    {
    public:

        CONSTRAINT(behavior);
        entity_behavior(ax::entity entity, ax::world& world) : properties(), entity(entity), world(world) { }
        inline const ax::property_map& get_behavior_properties() const { return properties; }
        inline ax::property_map& get_behavior_properties() { return properties; }
        virtual void update(int mode = 0) { mode; }

    protected:

        ENABLE_CAST(ax::entity_behavior, ax::castable);
        ax::property_map properties;
        ax::entity entity;
        ax::world& world;
    };
}

#endif
