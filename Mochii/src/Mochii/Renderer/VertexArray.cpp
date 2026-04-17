#include "mzpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Mochii/Platform/OpenGL/OpenGLVertexArray.h"

namespace Mochii {
	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI()) {
			case RendererAPI::None:    MI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:  return new OpenGLVertexArray();
		}

		MI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
