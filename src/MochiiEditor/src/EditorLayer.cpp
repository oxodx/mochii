#include "EditorLayer.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <optional>
#include "ImGuizmo.h"
#include "Mochii/Math/Math.h"
#include "Mochii/Scene/SceneSerializer.h"
#include "Mochii/Utils/PlatformUtils.h"

namespace Mochii {
EditorLayer::EditorLayer()
    : Layer("EditorLayer"),
      m_CameraController(1280.0f / 720.0f) {}

void EditorLayer::OnAttach() {
  MI_PROFILE_FUNCTION();

  m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

  FramebufferSpecification fbSpec;
  fbSpec.Attachments = {FramebufferTextureFormat::RGBA8,
                        FramebufferTextureFormat::RED_INTEGER,
                        FramebufferTextureFormat::Depth};
  fbSpec.Width = 1280;
  fbSpec.Height = 720;
  m_Framebuffer = Framebuffer::Create(fbSpec);

  m_ActiveScene = CreateRef<Scene>();

  m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

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
    m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x,
                                    (uint32_t)m_ViewportSize.y);
  }

  // Update
  if (m_ViewportFocused) m_CameraController.OnUpdate(ts);

  m_EditorCamera.OnUpdate(ts);

  // Bad Apple playback
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

  // Render
  Renderer2D::ResetStats();
  m_Framebuffer->Bind();
  RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
  RenderCommand::Clear();

  // Clear our entity ID attachment to -1
  m_Framebuffer->ClearAttachment(1, -1);

  // Update scene
  m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

  auto [mx, my] = ImGui::GetMousePos();
  mx -= m_ViewportBounds[0].x;
  my -= m_ViewportBounds[0].y;
  glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
  my = viewportSize.y - my;
  int mouseX = (int)mx;
  int mouseY = (int)my;

  if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x &&
      mouseY < (int)viewportSize.y) {
    int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
    MI_CORE_WARN("Pixel data = {0}", pixelData);
  }

  m_Framebuffer->Unbind();
}

void EditorLayer::OnImGuiRender() {
  MI_PROFILE_FUNCTION();

  static bool dockspaceOpen = true;
  static bool opt_fullscreen_persistant = true;
  bool opt_fullscreen = opt_fullscreen_persistant;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  if (opt_fullscreen) {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  }

  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
  ImGui::PopStyleVar();

  if (opt_fullscreen) ImGui::PopStyleVar(2);

  ImGuiIO& io = ImGui::GetIO();
  ImGuiStyle& style = ImGui::GetStyle();
  float minWinSizeX = style.WindowMinSize.x;
  style.WindowMinSize.x = 370.0f;
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  }

  style.WindowMinSize.x = minWinSizeX;

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New", "Ctrl+N")) NewScene();
      if (ImGui::MenuItem("Open...", "Ctrl+O")) OpenScene();
      if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) SaveSceneAs();
      ImGui::Separator();
      if (ImGui::MenuItem("Load Bad Apple Tilemap")) {
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
      if (ImGui::MenuItem("Exit")) Application::Get().Close();
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }

  m_SceneHierarchyPanel.OnImGuiRender();

  ImGui::Begin("Stats");

  auto stats = Renderer2D::GetStats();
  ImGui::Text("Renderer2D Stats:");
  ImGui::Text("Draw Calls: %d", stats.DrawCalls);
  ImGui::Text("Quads: %d", stats.QuadCount);
  ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
  ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
  ImGui::Separator();
  if (ImGui::Button("Load Bad Apple Tilemap")) {
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
  auto viewportOffset = ImGui::GetCursorPos();

  m_ViewportFocused = ImGui::IsWindowFocused();
  m_ViewportHovered = ImGui::IsWindowHovered();
  Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused &&
                                                  !m_ViewportHovered);

  ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
  m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

  uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
  ImGui::Image(reinterpret_cast<void*>(textureID),
               ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1},
               ImVec2{1, 0});

  auto windowSize = ImGui::GetWindowSize();
  ImVec2 minBound = ImGui::GetWindowPos();
  minBound.x += viewportOffset.x;
  minBound.y += viewportOffset.y;

  ImVec2 maxBound = {minBound.x + m_ViewportSize.x,
                     minBound.y + m_ViewportSize.y};
  m_ViewportBounds[0] = {minBound.x, minBound.y};
  m_ViewportBounds[1] = {maxBound.x, maxBound.y};

  Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
  if (selectedEntity && m_GizmoType != -1) {
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();

    float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
                      windowWidth, windowHeight);

    const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
    glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

    auto& tc = selectedEntity.GetComponent<TransformComponent>();
    glm::mat4 transform = tc.GetTransform();

    bool snap = Input::IsKeyPressed(Key::LeftControl);
    float snapValue = 0.5f;
    if (m_GizmoType == ImGuizmo::OPERATION::ROTATE) snapValue = 45.0f;

    float snapValues[3] = {snapValue, snapValue, snapValue};

    ImGuizmo::Manipulate(
        glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
        (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL,
        glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

    if (ImGuizmo::IsUsing()) {
      glm::vec3 translation, rotation, scale;
      Math::DecomposeTransform(transform, translation, rotation, scale);

      glm::vec3 deltaRotation = rotation - tc.Rotation;
      tc.Translation = translation;
      tc.Rotation += deltaRotation;
      tc.Scale = scale;
    }
  }

  ImGui::End();
  ImGui::PopStyleVar();

  ImGui::End();
}

void EditorLayer::OnEvent(Event& e) {
  m_CameraController.OnEvent(e);
  m_EditorCamera.OnEvent(e);

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
      break;
    }
    case Key::O: {
      if (control) OpenScene();
      break;
    }
    case Key::S: {
      if (control && shift) SaveSceneAs();
      break;
    }
    case Key::Q: {
      if (!ImGuizmo::IsUsing())
        m_GizmoType = -1;
      break;
    }
    case Key::W: {
      if (!ImGuizmo::IsUsing())
        m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
      break;
    }
    case Key::E: {
      if (!ImGuizmo::IsUsing())
        m_GizmoType = ImGuizmo::OPERATION::ROTATE;
      break;
    }
    case Key::R: {
      if (!ImGuizmo::IsUsing())
        m_GizmoType = ImGuizmo::OPERATION::SCALE;
      break;
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
  std::optional<std::string> filepath =
      FileDialogs::OpenFile("Mochii Scene (*.mochii)\0*.mochii\0");
  if (filepath) {
    m_ActiveScene = CreateRef<Scene>();
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x,
                                    (uint32_t)m_ViewportSize.y);
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);

    SceneSerializer serializer(m_ActiveScene);
    serializer.Deserialize(*filepath);
  }
}

void EditorLayer::SaveSceneAs() {
  std::optional<std::string> filepath =
      FileDialogs::SaveFile("Mochii Scene (*.mochii)\0*.mochii\0");
  if (filepath) {
    SceneSerializer serializer(m_ActiveScene);
    serializer.Serialize(*filepath);
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
      auto& tc = tile.GetComponent<TransformComponent>();
      const float x = (static_cast<float>(col) - halfWidth) * tileSize + tileSize * 0.5f;
      const float y =
          (halfHeight - static_cast<float>(row)) * tileSize - tileSize * 0.5f;
      tc.Translation = {x, y, 0.0f};
      tc.Scale = {tileSize, tileSize, 1.0f};
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