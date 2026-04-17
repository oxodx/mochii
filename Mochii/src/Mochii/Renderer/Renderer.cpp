#include "mzpch.h"
#include "Renderer.h"

namespace Mochii {
	Renderer::SceneData* Renderer::_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera) {
		_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray) {
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", _SceneData->ViewProjectionMatrix);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
