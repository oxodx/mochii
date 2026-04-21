#pragma once
#include "Mochii.h"
#include "BadApplePlayer.h"
#include "Panels/SceneHierarchyPanel.h"
#include <string>
#include <vector>

namespace Mochii {
class EditorLayer : public Layer {
 public:
  EditorLayer();
  virtual ~EditorLayer() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;

  void OnUpdate(Timestep ts) override;
  virtual void OnImGuiRender() override;
  void OnEvent(Event& e) override;

  void NewScene();
  void OpenScene();
  void SaveSceneAs();

  void ResetSceneForBadAppleTilemap();
  void UpdateBadAppleTilemapFrame(size_t frameIndex);

private:
  bool OnKeyPressed(KeyPressedEvent& e);

  Mochii::OrthographicCameraController m_CameraController;

  // Temp
  Ref<VertexArray> m_SquareVA;
  Ref<Shader> m_FlatColorShader;
  Ref<Framebuffer> m_Framebuffer;

  Ref<Scene> m_ActiveScene;
  Entity m_SquareEntity;
  Entity m_CameraEntity;
  Entity m_SecondCamera;

  bool m_PrimaryCamera = true;

  Ref<Texture2D> m_CheckerboardTexture;

  bool m_ViewportFocused = false, m_ViewportHovered = false;
  glm::vec2 m_ViewportSize = {0.0f, 0.0f};

  std::string m_BadAppleStatus = "Ready.";
  BadApplePlayer::TileVideo m_BadAppleTileVideo;
  std::vector<Entity> m_BadAppleTiles;
  float m_BadApplePlaybackTime = 0.0f;
  bool m_BadAppleTilemapPlaying = false;

  // Panels
  SceneHierarchyPanel m_SceneHierarchyPanel;
};
}  // namespace Mochii
