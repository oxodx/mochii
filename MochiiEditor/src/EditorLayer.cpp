#include "EditorLayer.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mochii/Scene/SceneSerializer.h"
#include "Mochii/Utils/PlatformUtils.h"

namespace Mochii {
EditorLayer::EditorLayer()
    : Layer("EditorLayer"),
      m_CameraController(1280.0f / 720.0f),
      m_SquareColor({0.2f, 0.3f, 0.8f, 1.0f}) {}

void EditorLayer::OnAttach() {
  MI_PROFILE_FUNCTION();

  m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

  Mochii::FramebufferSpecification fbSpec;
  fbSpec.Width = 1280;
  fbSpec.Height = 720;
  m_Framebuffer = Framebuffer::Create(fbSpec);

  m_ActiveScene = CreateRef<Scene>();

  m_SceneHierarchyPanel.SetContext(m_ActiveScene);
}

void EditorLayer::OnDetach() { MI_PROFILE_FUNCTION(); }

void EditorLayer::OnUpdate(Timestep ts) {
  MI_PROFILE_FUNCTION();

  // Resize
  if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
      m_ViewportSize.x > 0.0f &&
      m_ViewportSize.y > 0.0f &&  // zero sized framebuffer is invalid
      (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
    m_Framebuffer->Resize((uint32_t)m_ViewportSize.x,
                          (uint32_t)m_ViewportSize.y);
    m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x,
                                    (uint32_t)m_ViewportSize.y);
  }

  // Update
  if (m_ViewportFocused) m_CameraController.OnUpdate(ts);

  // Render
  Renderer2D::ResetStats();
  m_Framebuffer->Bind();
  RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
  RenderCommand::Clear();

  // Update scene
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
  // Shortcuts
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
}  // namespace Mochii
