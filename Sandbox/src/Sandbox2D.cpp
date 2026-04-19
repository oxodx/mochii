#include "Sandbox2D.h"
#include <imgui.h>
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach() {
  MI_PROFILE_FUNCTION();

  m_CheckerboardTexture =
      Mochii::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach() { MI_PROFILE_FUNCTION(); }

void Sandbox2D::OnUpdate(Mochii::Timestep ts) {
  MI_PROFILE_FUNCTION();

  // Update
  m_CameraController.OnUpdate(ts);
  m_SquarePos1.x += 1 * ts;

  // Render
  {
    MI_PROFILE_SCOPE("Renderer Prep");
    Mochii::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    Mochii::RenderCommand::Clear();
  }

  {
    MI_PROFILE_SCOPE("Renderer Draw");
    Mochii::Renderer2D::BeginScene(m_CameraController.GetCamera());
    Mochii::Renderer2D::DrawQuad({m_SquarePos1}, {0.8f, 0.8f},
                                 {m_SquareColor1});
    Mochii::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f},
                                 {m_SquareColor2});
    Mochii::Renderer2D::DrawQuad({-5.0f, -5.0f, -0.1f}, {10.0f, 10.0f},
                                 m_CheckerboardTexture, 10.0f);
    Mochii::Renderer2D::DrawQuad({-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f},
                                 m_CheckerboardTexture, 20.0f);
    Mochii::Renderer2D::EndScene();
  }
}

void Sandbox2D::OnImGuiRender() {
  MI_PROFILE_FUNCTION();

  ImGui::Begin("Settings");
  ImGui::ColorEdit4("Square Color 1", glm::value_ptr(m_SquareColor1));
  ImGui::ColorEdit4("Square Color 2", glm::value_ptr(m_SquareColor2));

  ImGui::End();
}

void Sandbox2D::OnEvent(Mochii::Event& e) { m_CameraController.OnEvent(e); }
