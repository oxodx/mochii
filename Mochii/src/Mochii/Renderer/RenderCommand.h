#pragma once
#include "RendererAPI.h"

namespace Mochii {
	class RenderCommand {
	public:
		inline static void Init() {
			_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color) {
			_RendererAPI->SetClearColor(color);
		}

		inline static void Clear() {
			_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
			_RendererAPI->DrawIndexed(vertexArray);
		}
	private:
		static Scope<RendererAPI> _RendererAPI;
	};
}
