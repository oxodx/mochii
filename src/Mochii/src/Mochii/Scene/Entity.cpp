#include "Entity.h"
#include "mzpch.h"

namespace Mochii {
Entity::Entity(entt::entity handle, Scene* scene)
    : m_EntityHandle(handle), m_Scene(scene) {}

bool Entity::IsValid() const {
  return m_Scene && m_EntityHandle != entt::null &&
         m_Scene->m_Registry.valid(m_EntityHandle);
}
}  // namespace Mochii
