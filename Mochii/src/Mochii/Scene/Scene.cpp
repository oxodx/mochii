#include "Scene.h"
#include <glm/glm.hpp>
#include "Components.h"
#include "Entity.h"
#include "Mochii/Renderer/Renderer2D.h"
#include "mzpch.h"

namespace Mochii {
Scene::Scene() : m_ViewportWidth(1280), m_ViewportHeight(720) {}

Scene::~Scene() {}

Entity Scene::CreateEntity(const std::string& name) {
  Entity entity = {m_Registry.create(), this};
  entity.AddComponent<UUIDComponent>(GenerateUUID());
  entity.AddComponent<TransformComponent>();
  auto& tag = entity.AddComponent<TagComponent>();
  tag.Tag = name.empty() ? "Entity" : name;
  return entity;
}

void Scene::DestroyEntity(Entity entity) { m_Registry.destroy(entity); }

void Scene::OnUpdate(Timestep ts) {
  // Update scripts
  {
    m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
      if (!nsc.Instance) {
        nsc.Instance = nsc.InstantiateScript();
        nsc.Instance->m_Entity = Entity{entity, this};

        nsc.Instance->OnCreate();
      }

      nsc.Instance->OnUpdate(ts);
    });
  }

  // Render 2D
  Camera* mainCamera = nullptr;
  glm::mat4 cameraTransform = glm::mat4(1.0f);
  {
    auto view = m_Registry.view<TransformComponent, CameraComponent>();
    for (auto entity : view) {
      auto& transform = view.get<TransformComponent>(entity);
      auto& camera = view.get<CameraComponent>(entity);

      if (camera.Primary) {
        mainCamera = &camera.Camera;
        cameraTransform = transform.GetTransform();
        break;
      }
    }
  }

  if (mainCamera) {
    Renderer2D::BeginScene(*mainCamera, cameraTransform);

    auto group = m_Registry.group<TransformComponent>(
        entt::get<SpriteRendererComponent>);
    for (auto entity : group) {
      auto& transform = group.get<TransformComponent>(entity);
      auto& sprite = group.get<SpriteRendererComponent>(entity);

      Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
    }

    Renderer2D::EndScene();
  }
}

void Scene::OnViewportResize(uint32_t width, uint32_t height) {
  m_ViewportWidth = width;
  m_ViewportHeight = height;

  // Resize our non-FixedAspectRatio cameras
  auto view = m_Registry.view<CameraComponent>();
  for (auto entity : view) {
    auto& cameraComponent = view.get<CameraComponent>(entity);
    if (!cameraComponent.FixedAspectRatio)
      cameraComponent.Camera.SetViewportSize(width, height);
  }
}

Entity Scene::GetPrimaryCameraEntity() {
  auto view = m_Registry.view<CameraComponent>();
  for (auto entity : view) {
    const auto& camera = view.get<CameraComponent>(entity);
    if (camera.Primary) return Entity{entity, this};
  }
  return {};
}

template <typename T>
void Scene::OnComponentAdded(Entity entity, T& component) {
  static_assert(false);
}

template <>
void Scene::OnComponentAdded<TransformComponent>(
    Entity entity, TransformComponent& component) {}

template <>
void Scene::OnComponentAdded<CameraComponent>(Entity entity,
                                              CameraComponent& component) {
  component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
}

template <>
void Scene::OnComponentAdded<SpriteRendererComponent>(
    Entity entity, SpriteRendererComponent& component) {}

template <>
void Scene::OnComponentAdded<TagComponent>(Entity entity,
                                           TagComponent& component) {}

template <>
void Scene::OnComponentAdded<NativeScriptComponent>(
    Entity entity, NativeScriptComponent& component) {}

template <>
void Scene::OnComponentAdded<UUIDComponent>(Entity entity,
                                            UUIDComponent& component) {}
}  // namespace Mochii
