#include "Mochii/Renderer/RendererAPI.h"
#include "Mochii/Core/Core.h"
#include "Mochii/Platform/OpenGL/OpenGLRendererAPI.h"
#include "mzpch.h"

namespace Mochii {
RendererAPI::API RendererAPI::_API = RendererAPI::API::OpenGL;

Scope<RendererAPI> RendererAPI::Create() {
  switch (_API) {
    case RendererAPI::API::None:
      MI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return CreateScope<OpenGLRendererAPI>();
  }

  MI_CORE_ASSERT(false, "Unknown RendererAPI!");
  return nullptr;
}
}  // namespace Mochii
