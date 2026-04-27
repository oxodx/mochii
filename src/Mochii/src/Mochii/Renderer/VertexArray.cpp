#include "Mochii/Renderer/VertexArray.h"
#include "Mochii/Core/Base.h"
#include "Mochii/Platform/OpenGL/OpenGLVertexArray.h"
#include "Mochii/Renderer/Renderer.h"
#include "mzpch.h"

namespace Mochii {
Ref<VertexArray> VertexArray::Create() {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      MI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLVertexArray>();
  }

  MI_CORE_ASSERT(false, "Unknown RendererAPI!");
  return nullptr;
}
}  // namespace Mochii
