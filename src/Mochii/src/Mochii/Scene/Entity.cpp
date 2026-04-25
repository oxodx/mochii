#include "Entity.h"
#include "mzpch.h"

namespace Mochii {
Entity::Entity(entt::entity handle, Scene* scene)
    : m_EntityHandle(handle), m_Scene(scene) {}
}  // namespace Mochii
