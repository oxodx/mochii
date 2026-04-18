#include "Renderer.h"
#include "Mochii/Platform/OpenGL/OpenGLShader.h"
#include "mzpch.h"

namespace Mochii {
Scope<Renderer::SceneData> Renderer::_SceneData =
    CreateScope<Renderer::SceneData>();

void Renderer::Init() { RenderCommand::Init(); }

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
  RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(OrthographicCamera& camera) {
  _SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<Shader>& shader,
                      const std::shared_ptr<VertexArray>& vertexArray,
                      const glm::mat4& transform) {
  shader->Bind();
  std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4(
      "u_ViewProjection", _SceneData->ViewProjectionMatrix);
  std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4(
      "u_Transform", transform);

  vertexArray->Bind();
  RenderCommand::DrawIndexed(vertexArray);
}
}  // namespace Mochii
