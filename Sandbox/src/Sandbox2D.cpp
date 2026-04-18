#include "Sandbox2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach() {
  m_CheckerboardTexture =
      Mochii::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(Mochii::Timestep ts) {
  // Update
  m_CameraController.OnUpdate(ts);

  // Render
  Mochii::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
  Mochii::RenderCommand::Clear();

  Mochii::Renderer2D::BeginScene(m_CameraController.GetCamera());
  Mochii::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {m_SquareColor1});
  Mochii::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, {m_SquareColor2});
  Mochii::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {10.0f, 10.0f},
                               m_CheckerboardTexture);
  Mochii::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender() {
  ImGui::Begin("Settings");
  ImGui::ColorEdit4("Square Color 1", glm::value_ptr(m_SquareColor1));
  ImGui::ColorEdit4("Square Color 2", glm::value_ptr(m_SquareColor2));
  ImGui::End();
}

void Sandbox2D::OnEvent(Mochii::Event& e) { m_CameraController.OnEvent(e); }
