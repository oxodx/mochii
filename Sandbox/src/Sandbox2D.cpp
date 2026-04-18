#include "Sandbox2D.h"
#include <Mochii/Platform/OpenGL/OpenGLShader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach() {}

void Sandbox2D::OnDetach() {}

void Sandbox2D::OnUpdate(Mochii::Timestep ts) {
  // Update
  m_CameraController.OnUpdate(ts);

  // Render
  Mochii::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
  Mochii::RenderCommand::Clear();

  Mochii::Renderer2D::BeginScene(m_CameraController.GetCamera());
  Mochii::Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, m_SquareColor);
  Mochii::Renderer2D::EndScene();

  // TODO: Add these functions - Shader::SetMat4, Shader::SetFloat4
  // std::dynamic_pointer_cast<Mochii::OpenGLShader>(m_FlatColorShader)->Bind();
  // std::dynamic_pointer_cast<Mochii::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color",
  // m_SquareColor);
}

void Sandbox2D::OnImGuiRender() {
  ImGui::Begin("Settings");
  ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
  ImGui::End();
}

void Sandbox2D::OnEvent(Mochii::Event& e) { m_CameraController.OnEvent(e); }
