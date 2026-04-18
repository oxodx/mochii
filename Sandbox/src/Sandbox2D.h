#pragma once
#include <Mochii.h>

class Sandbox2D : public Mochii::Layer {
 public:
  Sandbox2D();
  virtual ~Sandbox2D() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;

  void OnUpdate(Mochii::Timestep ts) override;
  virtual void OnImGuiRender() override;
  void OnEvent(Mochii::Event& e) override;

 private:
  Mochii::OrthographicCameraController m_CameraController;

  // Temp
  Mochii::Ref<Mochii::VertexArray> m_SquareVA;
  Mochii::Ref<Mochii::Shader> m_FlatColorShader;

  Mochii::Ref<Mochii::Texture2D> m_CheckerboardTexture;

  glm::vec4 m_SquareColor1 = {0.8f, 0.2f, 0.3f, 1.0f};
  glm::vec4 m_SquareColor2 = {0.2f, 0.3f, 0.8f, 1.0f};
};
