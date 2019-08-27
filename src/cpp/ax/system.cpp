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

    ax::transform* world::try_get_transform(const ax::address& address)
    {
        VAL& transforms_iter = systems.find("transforms");
        if (transforms_iter != systems.end())
        {
            VAL& transforms = ax::cast<ax::system_t<ax::transform>>(transforms_iter->second);
            return transforms->try_get_component(address);
        }
        return nullptr;
    }

    ax::entity_component* world::try_get_entity(const ax::address& address)
    {
        VAL& entities_iter = systems.find("entities");
        if (entities_iter != systems.end())
        {
            VAL& entities = ax::cast<ax::system_t<ax::entity_component>>(entities_iter->second);
            return entities->try_get_component(address);
        }
        return nullptr;
    }

    ax::entity_behavior_component* world::try_get_entity_behavior(const ax::address& address)
    {
        VAL& behaviors_iter = systems.find("entity_behaviors");
        if (behaviors_iter != systems.end())
        {
            VAL& behaviors = ax::cast<ax::system_t<ax::entity_behavior_component>>(behaviors_iter->second);
            return behaviors->try_get_component(address);
        }
        return nullptr;
    }

    bool world::entity_exists(const ax::address& address)
    {
        return try_get_entity(address) != nullptr;
    }

    ax::component* world::try_add_component(const ax::name& system_name, const ax::address& address)
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
                    VAL& system_opt = ax::try_cast<ax::system_n>(system_iter->second);
                    if (system_opt)
                    {
                        VAR& result = system_opt->add_component(address);
                        entity_opt->components[system_name] = &result;
                        return &result;
                    }
                }
            }
        }
        return nullptr;
    }

    bool world::try_remove_component(const ax::name& system_name, const ax::address& address)
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
                    VAL& system_opt = ax::try_cast<ax::system_n>(system_iter->second);
                    if (system_opt)
                    {
                        VAR result = system_opt->remove_component(address);
                        entity_opt->components.erase(system_name);
                        return result;
                    }
                }
            }
        }
        return false;
    }

    ax::entity world::create_entity(const ax::address& address)
    {
        VAL& entities_iter = systems.find("entities");
        if (entities_iter != systems.end())
        {
            VAL& entities = ax::cast<ax::system_t<ax::entity_component>>(entities_iter->second);
            VAR* entity_opt = entities->try_get_component(address);
            if (entity_opt)
            {
                try_add_entity(address);
                try_add_component<ax::transform>("transform", address);
            }
        }
        return ax::entity(address, *this);
    }

    bool world::destroy_entity(const ax::address& address)
    {
        VAL& entities_iter = systems.find("entities");
        if (entities_iter != systems.end())
        {
            VAL& entities = ax::cast<ax::system_t<ax::entity_component>>(entities_iter->second);
            VAR* entity_opt = entities->try_get_component(address);
            if (entity_opt)
            {
                try_remove_component("transform", address);
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

    ax::entity_component* world::try_add_entity(const ax::address& address)
    {
        VAL& entities_iter = systems.find("entities");
        if (entities_iter != systems.end())
        {
            VAL& entities = ax::cast<ax::system_t<ax::entity_component>>(entities_iter->second);
            VAR* entity_opt = entities->try_get_component(address);
            if (!entity_opt) return &entities->add_component(address);
        }
        return nullptr;
    }

    bool world::try_remove_entity(const ax::address& address)
    {
        VAL& entities_iter = systems.find("entities");
        if (entities_iter != systems.end())
        {
            VAL& entities = ax::cast<ax::system_t<ax::entity_component>>(entities_iter->second);
            VAR* entity_opt = entities->try_get_component(address);
            if (entity_opt) return entities->remove_component(address);
        }
        return false;
    }

    const ax::property_map* entity::try_get_behavior_properties() const
    {
        VAL& behavior_component_opt = try_get_behavior();
        if (behavior_component_opt) return &((*behavior_component_opt).get_behavior_properties());
        return nullptr;
    }

    ax::property_map* entity::try_get_behavior_properties()
    {
        VAL& behavior_component_opt = try_get_behavior();
        if (behavior_component_opt) return &((*behavior_component_opt).get_behavior_properties());
        return nullptr;
    }

    const ax::property_map& entity::get_behavior_properties() const
    {
        VAL* properties = try_get_behavior_properties();
        if (properties) return *properties;
        throw std::logic_error("Entity does not have properties.");
    }

    ax::property_map& entity::get_behavior_properties()
    {
        VAR* properties = try_get_behavior_properties();
        if (properties) return *properties;
        throw std::logic_error("Entity does not have properties.");
    }

    std::shared_ptr<const ax::entity_behavior> entity::try_get_behavior() const
    {
        VAR* behavior_component_opt = world.try_get_entity_behavior(get_address());
        if (behavior_component_opt) return behavior_component_opt->behavior;
        return nullptr;
    }

    std::shared_ptr<ax::entity_behavior> entity::try_get_behavior()
    {
        VAR* behavior_component_opt = world.try_get_entity_behavior(get_address());
        if (behavior_component_opt) return behavior_component_opt->behavior;
        return nullptr;
    }

    const ax::entity_behavior& entity::get_behavior() const
    {
        VAL& behavior = try_get_behavior();
        if (behavior) return *behavior;
        throw std::logic_error("Entity does not have behavior.");
    }

    ax::entity_behavior& entity::get_behavior()
    {
        VAL& behavior = try_get_behavior();
        if (behavior) return *behavior;
        throw std::logic_error("Entity does not have behavior.");
    }

    const ax::transform& entity::get_transform() const
    {
        return const_cast<entity*>(this)->get_transform();
    }

    ax::transform& entity::get_transform()
    {
        if (transform_cache && transform_cache_index == transform_cache->index) return *transform_cache;
        VAR& transform = *world.try_get_transform(get_address()); // always exists
        transform_cache = &transform;
        transform_cache_index = transform.index;
        return transform;
    }

    ax::transform& entity::set_transform(const ax::transform& transform)
    {
        return get_transform() = transform;
    }

    bool entity::exists() const
    {
        return world.entity_exists(get_address());
    }
}
