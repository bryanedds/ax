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
    { }

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

    bool world::entity_exists(const ax::address& address)
    {
        return try_get_entity(address) != nullptr;
    }

    ax::component* world::try_add_component(const ax::name_t& system_name, const ax::address& address)
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
                    VAR& system = system_iter->second;
                    VAR& result = system->add_component(address);
                    std::insert_or_assign(entity_opt->components, system_name, &result);
                    return &result;
                }
            }
        }
        return nullptr;
    }

    bool world::try_remove_component(const ax::name_t& system_name, const ax::address& address)
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
                    VAR& system = system_iter->second;
                    VAR result = system->remove_component(address);
                    entity_opt->components.erase(system_name);
                    return result;
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

    ax::system_ptr world::try_add_system(const ax::name_t& name, ax::system_ptr system)
    {
        std::insert_or_assign(systems, name, system);
        return system;
    }

    bool world::remove_system(const ax::name_t& name)
    {
        return systems.erase(name) != 0;
    }

    void world::initialize_systems()
    {
        initialize_systems_impl(*this);
    }

    void world::update_systems()
    {
        update_systems_impl(*this);
    }

    void world::clean_up_systems()
    {
        clean_up_systems_impl(*this);
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
}
