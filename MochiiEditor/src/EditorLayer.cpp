#include "EditorLayer.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mochii/Scene/SceneSerializer.h"
#include "Mochii/Utils/PlatformUtils.h"

namespace Mochii {
EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f) {}

void EditorLayer::OnAttach() {
  MI_PROFILE_FUNCTION();

  Mochii::FramebufferSpecification fbSpec;
  fbSpec.Width = 1280;
  fbSpec.Height = 720;
  m_Framebuffer = Framebuffer::Create(fbSpec);

  m_ActiveScene = CreateRef<Scene>();

  m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
  auto& cameraComponent = m_CameraEntity.AddComponent<CameraComponent>();
  cameraComponent.Primary = true;

  m_SceneHierarchyPanel.SetContext(m_ActiveScene);
}

void EditorLayer::OnDetach() { MI_PROFILE_FUNCTION(); }

void EditorLayer::OnUpdate(Timestep ts) {
  MI_PROFILE_FUNCTION();

  if (m_ViewportFocused) m_CameraController.OnUpdate(ts);
  if (m_BadAppleTilemapPlaying && m_BadAppleTileVideo.Width > 0 &&
      m_BadAppleTileVideo.Height > 0 && !m_BadAppleTileVideo.Frames.empty()) {
    const size_t framePixelCount =
        static_cast<size_t>(m_BadAppleTileVideo.Width) * m_BadAppleTileVideo.Height;
    const size_t frameCount = m_BadAppleTileVideo.Frames.size() / framePixelCount;
    if (frameCount > 0) {
      m_BadApplePlaybackTime += ts;
      const size_t frameIndex =
          static_cast<size_t>(m_BadApplePlaybackTime * m_BadAppleTileVideo.FPS) %
          frameCount;
      UpdateBadAppleTilemapFrame(frameIndex);
    }
  }

  Renderer2D::ResetStats();
  m_Framebuffer->Bind();
  RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
  RenderCommand::Clear();

  m_ActiveScene->OnUpdate(ts);

  m_Framebuffer->Unbind();
}

void EditorLayer::OnImGuiRender() {
  MI_PROFILE_FUNCTION();

  m_SceneHierarchyPanel.OnImGuiRender();

  ImGui::Begin("Stats");

  auto stats = Renderer2D::GetStats();
  ImGui::Text("Renderer2D Stats:");
  ImGui::Text("Draw Calls: %d", stats.DrawCalls);
  ImGui::Text("Quads: %d", stats.QuadCount);
  ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
  ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
  ImGui::Separator();
  if (ImGui::Button("Load Bad Apple Tilemap Level")) {
    if (!Mochii::BadApplePlayer::LoadTileVideo(m_BadAppleTileVideo,
                                               m_BadAppleStatus))
      MI_ERROR("{}", m_BadAppleStatus);
    else {
      ResetSceneForBadAppleTilemap();
      m_BadApplePlaybackTime = 0.0f;
      m_BadAppleTilemapPlaying = true;
      MI_INFO("{}", m_BadAppleStatus);
    }
  }
  ImGui::Separator();
  if (ImGui::Button(m_BadAppleTilemapPlaying ? "Pause Tilemap" : "Play Tilemap")) {
    m_BadAppleTilemapPlaying = !m_BadAppleTilemapPlaying;
  }
  ImGui::SameLine();
  if (ImGui::Button("Restart Tilemap")) {
    m_BadApplePlaybackTime = 0.0f;
    UpdateBadAppleTilemapFrame(0);
  }
  ImGui::Separator();
  ImGui::TextWrapped("Bad Apple: %s", m_BadAppleStatus.c_str());

  ImGui::End();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  ImGui::Begin("Viewport");

  m_ViewportFocused = ImGui::IsWindowFocused();
  m_ViewportHovered = ImGui::IsWindowHovered();
  Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused ||
                                                  !m_ViewportHovered);

  ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
  const glm::vec2 newViewportSize(viewportPanelSize.x, viewportPanelSize.y);
  if (newViewportSize.x > 0.0f && newViewportSize.y > 0.0f &&
      m_ViewportSize != newViewportSize) {
    m_Framebuffer->Resize((uint32_t)newViewportSize.x,
                          (uint32_t)newViewportSize.y);
    m_ViewportSize = newViewportSize;

    m_CameraController.OnResize(newViewportSize.x, newViewportSize.y);
    m_ActiveScene->OnViewportResize((uint32_t)newViewportSize.x,
                                      (uint32_t)newViewportSize.y);
  }
  uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
  ImGui::Image(reinterpret_cast<void*>(textureID),
               ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1},
               ImVec2{1, 0});
  ImGui::End();
  ImGui::PopStyleVar();
}

void EditorLayer::OnEvent(Event& e) {
  m_CameraController.OnEvent(e);

  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<KeyPressedEvent>(
      MI_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& e) {
  if (e.GetRepeatCount() > 0) return false;

  bool control = Input::IsKeyPressed(Key::LeftControl) ||
                 Input::IsKeyPressed(Key::RightControl);
  bool shift = Input::IsKeyPressed(Key::LeftShift) ||
               Input::IsKeyPressed(Key::RightShift);
  switch (e.GetKeyCode()) {
    case Key::N: {
      if (control) NewScene();
      return true;
    }
    case Key::O: {
      if (control) OpenScene();
      return true;
    }
    case Key::S: {
      if (control && shift) SaveSceneAs();
      return true;
    }
  }
  return false;
}

void EditorLayer::NewScene() {
  m_ActiveScene = CreateRef<Scene>();
  m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x,
                                  (uint32_t)m_ViewportSize.y);
  m_SceneHierarchyPanel.SetContext(m_ActiveScene);
}

void EditorLayer::OpenScene() {
  std::string filepath =
      FileDialogs::OpenFile("Mochii Scene (*.mochii)\0*.mochii\0");
  if (!filepath.empty()) {
    m_ActiveScene = CreateRef<Scene>();
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x,
                                    (uint32_t)m_ViewportSize.y);
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);

    SceneSerializer serializer(m_ActiveScene);
    serializer.Deserialize(filepath);
  }
}

void EditorLayer::SaveSceneAs() {
  std::string filepath =
      FileDialogs::SaveFile("Mochii Scene (*.mochii)\0*.mochii\0");
  if (!filepath.empty()) {
    SceneSerializer serializer(m_ActiveScene);
    serializer.Serialize(filepath);
  }
}

void EditorLayer::ResetSceneForBadAppleTilemap() {
  m_ActiveScene = CreateRef<Scene>();
  m_BadAppleTiles.clear();

  m_CameraEntity = m_ActiveScene->CreateEntity("BadAppleCamera");
  auto& cameraComponent = m_CameraEntity.AddComponent<CameraComponent>();
  cameraComponent.Primary = true;
  cameraComponent.Camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);

  constexpr float tileSize = 0.2f;
  const float worldHeight = static_cast<float>(m_BadAppleTileVideo.Height) * tileSize;
  cameraComponent.Camera.SetOrthographic(worldHeight, -1.0f, 1.0f);

  const float halfWidth = static_cast<float>(m_BadAppleTileVideo.Width) * 0.5f;
  const float halfHeight = static_cast<float>(m_BadAppleTileVideo.Height) * 0.5f;
  for (uint32_t row = 0; row < m_BadAppleTileVideo.Height; ++row) {
    for (uint32_t col = 0; col < m_BadAppleTileVideo.Width; ++col) {
      Entity tile = m_ActiveScene->CreateEntity("BadAppleTile");
      auto& transform = tile.GetComponent<TransformComponent>();
      const float x = (static_cast<float>(col) - halfWidth) * tileSize + tileSize * 0.5f;
      const float y =
          (halfHeight - static_cast<float>(row)) * tileSize - tileSize * 0.5f;
      transform.Translation = {x, y, 0.0f};
      transform.Scale = {tileSize, tileSize, 1.0f};
      tile.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 0.0f, 0.0f, 1.0f});
      m_BadAppleTiles.push_back(tile);
    }
  }

  UpdateBadAppleTilemapFrame(0);
  m_SceneHierarchyPanel.SetContext(m_ActiveScene);
}

void EditorLayer::UpdateBadAppleTilemapFrame(size_t frameIndex) {
  const size_t framePixelCount =
      static_cast<size_t>(m_BadAppleTileVideo.Width) * m_BadAppleTileVideo.Height;
  if (framePixelCount == 0) return;

  const size_t frameCount = m_BadAppleTileVideo.Frames.size() / framePixelCount;
  if (frameCount == 0) return;
  const size_t clampedFrameIndex = frameIndex % frameCount;
  const size_t frameOffset = clampedFrameIndex * framePixelCount;

  const size_t tileCount = std::min(m_BadAppleTiles.size(), framePixelCount);
  for (size_t i = 0; i < tileCount; ++i) {
    const float value = m_BadAppleTileVideo.Frames[frameOffset + i] > 0 ? 1.0f : 0.0f;
    auto& sprite = m_BadAppleTiles[i].GetComponent<SpriteRendererComponent>();
    sprite.Color = {value, value, value, 1.0f};
  }
}
}  // namespace Mochii