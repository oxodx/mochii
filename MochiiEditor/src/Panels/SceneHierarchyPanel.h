#pragma once
#include "Mochii/Core/Base.h"
#include "Mochii/Scene/Entity.h"
#include "Mochii/Scene/Scene.h"

namespace Mochii {
class SceneHierarchyPanel {
 public:
  SceneHierarchyPanel() = default;
  SceneHierarchyPanel(const Ref<Scene>& scene);

  void SetContext(const Ref<Scene>& scene);

  void OnImGuiRender();

  Entity GetSelectedEntity() const { return m_SelectionContext; }

 private:
  void DrawEntityNode(Entity entity);
  void DrawComponents(Entity entity);

 private:
  Ref<Scene> m_Context;
  Entity m_SelectionContext;
};
}  // namespace Mochii
