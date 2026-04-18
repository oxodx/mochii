#include "Mochii/Renderer/Renderer.h"
#include "Mochii/Platform/OpenGL/OpenGLShader.h"
#include "Mochii/Renderer/Renderer2D.h"
#include "mzpch.h"

namespace Mochii {
Scope<Renderer::SceneData> Renderer::_SceneData =
    CreateScope<Renderer::SceneData>();

void Renderer::Init() {
  RenderCommand::Init();
  Renderer2D::Init();
}

void Renderer::Shutdown() { Renderer2D::Shutdown(); }

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
  RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(OrthographicCamera& camera) {
  _SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<Shader>& shader,
                      const Ref<VertexArray>& vertexArray,
                      const glm::mat4& transform) {
  shader->Bind();
  shader->SetMat4("u_ViewProjection", _SceneData->ViewProjectionMatrix);
  shader->SetMat4("u_Transform", transform);

  vertexArray->Bind();
  RenderCommand::DrawIndexed(vertexArray);
}
}  // namespace Mochii
