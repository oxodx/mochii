#include "Scene.h"
#include <glm/glm.hpp>
#include "Components.h"
#include "Entity.h"
#include "Mochii/Renderer/Renderer2D.h"
#include "mzpch.h"

namespace Mochii {
static void DoMath(const glm::mat4& transform) {}

static void OnTransformConstruct(entt::registry& registry,
                                 entt::entity entity) {}

Scene::Scene() {}

Scene::~Scene() {}

Entity Scene::CreateEntity(const std::string& name) {
  Entity entity = {m_Registry.create(), this};
  entity.AddComponent<TransformComponent>();
  auto& tag = entity.AddComponent<TagComponent>();
  tag.Tag = name.empty() ? "Entity" : name;
  return entity;
}

void Scene::OnUpdate(Timestep ts) {
  // Render 2D
  Camera* mainCamera = nullptr;
  glm::mat4* cameraTransform = nullptr;
  {
    auto group = m_Registry.view<TransformComponent, CameraComponent>();
    for (auto entity : group) {
      auto& transform = group.get<TransformComponent>(entity);
      auto& camera = group.get<CameraComponent>(entity);

      if (camera.Primary) {
        mainCamera = &camera.Camera;
        cameraTransform = &transform.Transform;
        break;
      }
    }
  }

  if (mainCamera) {
    Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

    auto group = m_Registry.group<TransformComponent>(
        entt::get<SpriteRendererComponent>);
    for (auto entity : group) {
      auto& transform = group.get<TransformComponent>(entity);
      auto& sprite = group.get<SpriteRendererComponent>(entity);

      Renderer2D::DrawQuad(transform, sprite.Color);
    }

    Renderer2D::EndScene();
  }
}
}  // namespace Mochii
