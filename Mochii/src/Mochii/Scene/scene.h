#pragma once
#include "Mochii/Core/Timestep.h"
#include <entt/entt.hpp>

namespace Mochii {
class Entity;

class Scene {
 public:
  Scene();
  ~Scene();

  Entity CreateEntity(const std::string& name = std::string());

  void OnUpdate(Timestep ts);

 private:
  entt::registry m_Registry;

  friend class Entity;
};
}  // namespace Mochii
