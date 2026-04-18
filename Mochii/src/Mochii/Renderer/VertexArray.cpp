#include "VertexArray.h"
#include "Mochii/Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer.h"
#include "mzpch.h"

namespace Mochii {
VertexArray* VertexArray::Create() {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      MI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return new OpenGLVertexArray();
  }

  MI_CORE_ASSERT(false, "Unknown RendererAPI!");
  return nullptr;
}
}  // namespace Mochii
