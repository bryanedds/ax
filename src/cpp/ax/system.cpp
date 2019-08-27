#include "../../hpp/ax/impl/system.hpp"

namespace ax
{
    world::world(
        std::function<void(ax::world& world)> initialize_systems_impl,
        std::function<void(ax::world& world)> update_systems_impl,
        std::function<void(ax::world& world)> clean_up_systems_impl) :
        initialize_systems_impl(initialize_systems_impl),
        update_systems_impl(update_systems_impl),
        clean_up_systems_impl(clean_up_systems_impl)
    {
        initialize_systems_impl(*this);
    }

    world::~world()
    {
        clean_up_systems_impl(*this);
    }

    ax::entity_state* world::try_get_entity_state(const ax::address& address)
    {
        VAL& entity_states_iter = systems.find("entity_state");
        if (entity_states_iter != systems.end())
        {
            VAL& entity_states = ax::cast<ax::system_t<ax::entity_state>>(entity_states_iter->second);
            return entity_states->try_get_component(address);
        }
        return nullptr;
    }

    ax::entity_behavior_component* world::try_get_entity_behavior(const ax::address& address)
    {
        VAL& behaviors_iter = systems.find("entity_behavior");
        if (behaviors_iter != systems.end())
        {
            VAL& behaviors = ax::cast<ax::system_t<ax::entity_behavior_component>>(behaviors_iter->second);
            return behaviors->try_get_component(address);
        }
        return nullptr;
    }

    bool world::entity_exists(const ax::address& address)
    {
        return try_get_entity_state(address);
    }

    ax::component* world::try_add_component(const ax::name& system_name, const ax::address& address)
    {
        VAL& entity_states_iter = systems.find("entity_state");
        if (entity_states_iter != systems.end())
        {
            VAL& entity_states = ax::cast<ax::system_t<ax::entity_state>>(entity_states_iter->second);
            VAR* entity_state_opt = entity_states->try_get_component(address);
            if (entity_state_opt)
            {
                VAL& system_iter = systems.find(system_name);
                if (system_iter != systems.end())
                {
                    VAL& system_opt = ax::try_cast<ax::system_n>(system_iter->second);
                    if (system_opt)
                    {
                        VAR& result = system_opt->add_component(address);
                        entity_state_opt->components[system_name] = &result;
                        return &result;
                    }
                }
            }
        }
        return nullptr;
    }

    bool world::try_remove_component(const ax::name& system_name, const ax::address& address)
    {
        VAL& entity_states_iter = systems.find("entity_state");
        if (entity_states_iter != systems.end())
        {
            VAL& entity_states = ax::cast<ax::system_t<ax::entity_state>>(entity_states_iter->second);
            VAR* entity_state_opt = entity_states->try_get_component(address);
            if (entity_state_opt)
            {
                VAL& system_iter = systems.find(system_name);
                if (system_iter != systems.end())
                {
                    VAL& system_opt = ax::try_cast<ax::system_n>(system_iter->second);
                    if (system_opt)
                    {
                        VAR result = system_opt->remove_component(address);
                        entity_state_opt->components.erase(system_name);
                        return result;
                    }
                }
            }
        }
        return false;
    }

    ax::entity world::create_entity(const ax::address& address)
    {
        VAL& entity_states_iter = systems.find("entity_state");
        if (entity_states_iter != systems.end())
        {
            VAL& entity_states = ax::cast<ax::system_t<ax::entity_state>>(entity_states_iter->second);
            VAR* entity_state_opt = entity_states->try_get_component(address);
            if (!entity_state_opt)
            {
                VAR* entity_state_opt = try_add_entity(address);
                if (entity_state_opt) return ax::entity(address, *this);
                throw std::runtime_error("Could not create entity.");
            }
            return ax::entity(address, *this);
        }
        throw std::runtime_error("Could not create entity.");
    }

    bool world::destroy_entity(const ax::address& address)
    {
        VAL& entity_states_iter = systems.find("entity_state");
        if (entity_states_iter != systems.end())
        {
            VAL& entity_states = ax::cast<ax::system_t<ax::entity_state>>(entity_states_iter->second);
            VAR* entity_state_opt = entity_states->try_get_component(address);
            if (entity_state_opt)
            {
                try_remove_component("entity_state", address);
                return try_remove_entity(address);
            }
        }
        return false;
    }

    std::shared_ptr<ax::system> world::try_add_system(const ax::name& name, std::shared_ptr<ax::system> system)
    {
        systems[name] = system;
        return system;
    }

    bool world::remove_system(const ax::name& name)
    {
        return systems.erase(name) != 0;
    }

    void world::update_systems()
    {
        update_systems_impl(*this);
    }

    ax::entity_state* world::try_add_entity(const ax::address& address)
    {
        VAL& entity_states_iter = systems.find("entity_state");
        if (entity_states_iter != systems.end())
        {
            VAL& entity_states = ax::cast<ax::system_t<ax::entity_state>>(entity_states_iter->second);
            VAR* entity_state_opt = entity_states->try_get_component(address);
            if (!entity_state_opt) return &entity_states->add_component(address);
        }
        return nullptr;
    }

    bool world::try_remove_entity(const ax::address& address)
    {
        VAL& entity_states_iter = systems.find("entity_state");
        if (entity_states_iter != systems.end())
        {
            VAL& entity_states = ax::cast<ax::system_t<ax::entity_state>>(entity_states_iter->second);
            VAR* entity_state_opt = entity_states->try_get_component(address);
            if (entity_state_opt) return entity_states->remove_component(address);
        }
        return false;
    }

    const ax::property_map* entity::try_get_behavior_properties() const
    {
        return const_cast<entity*>(this)->try_get_behavior_properties();
    }

    ax::property_map* entity::try_get_behavior_properties()
    {
        VAL& behavior_component_opt = try_get_behavior();
        if (behavior_component_opt) return &((*behavior_component_opt).get_behavior_properties());
        return nullptr;
    }

    const ax::property_map& entity::get_behavior_properties() const
    {
        return const_cast<entity*>(this)->get_behavior_properties();
    }

    ax::property_map& entity::get_behavior_properties()
    {
        VAR* properties = try_get_behavior_properties();
        if (properties) return *properties;
        throw std::runtime_error("Entity does not have properties.");
    }

    std::shared_ptr<const ax::entity_behavior> entity::try_get_behavior() const
    {
        return const_cast<entity*>(this)->try_get_behavior();
    }

    std::shared_ptr<ax::entity_behavior> entity::try_get_behavior()
    {
        VAR* behavior_component_opt = world.try_get_entity_behavior(get_address());
        if (behavior_component_opt) return behavior_component_opt->behavior;
        return nullptr;
    }

    const ax::entity_behavior& entity::get_behavior() const
    {
        return const_cast<entity*>(this)->get_behavior();
    }

    ax::entity_behavior& entity::get_behavior()
    {
        VAL& behavior = try_get_behavior();
        if (behavior) return *behavior;
        throw std::runtime_error("Entity does not have behavior.");
    }

    const ax::entity_state* entity::try_get_entity_state() const
    {
        return const_cast<entity*>(this)->try_get_entity_state();
    }

    ax::entity_state* entity::try_get_entity_state()
    {
        VAR* entity_state_opt = entity_state_cache.try_get();
        if (!entity_state_opt)
        {
            VAR* entity_state_opt = world.try_get_entity_state(get_address());
            if (entity_state_opt)
            {
                entity_state_cache.reset(entity_state_opt);
                return entity_state_opt;
            }
            return nullptr;
        }
        return entity_state_opt;
    }

    const ax::entity_state& entity::get_entity_state() const
    {
        return const_cast<entity*>(this)->get_entity_state();
    }

    ax::entity_state& entity::get_entity_state()
    {
        VAR* entity_state_opt = try_get_entity_state();
        if (entity_state_opt) return *entity_state_opt;
        throw std::runtime_error("Could not get entity state.");
    }

    bool entity::exists() const
    {
        // OPTIMIZATION: checking entity state cache is faster than a fresh look-up.
        return try_get_entity_state();
    }
}
