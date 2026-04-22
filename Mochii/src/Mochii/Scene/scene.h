#pragma once
#include <entt/entt.hpp>
#include "Mochii/Core/Timestep.h"

namespace Mochii {
class Entity;

class Scene {
 public:
  Scene();
  ~Scene();

  const std::string& GetName() const { return m_Name; }
  void SetName(const std::string& name) { m_Name = name; }

  Entity CreateEntity(const std::string& name = std::string());
  void DestroyEntity(Entity entity);

  void OnUpdate(Timestep ts);
  void OnViewportResize(uint32_t width, uint32_t height);

 private:
  template <typename T>
  void OnComponentAdded(Entity entity, T& component);

  std::string m_Name = "Untitled";
  entt::registry m_Registry;
  uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

  friend class Entity;
  friend class SceneSerializer;
  friend class SceneHierarchyPanel;
};
}  // namespace Mochii
