#include "Mochii/Renderer/GraphicsContext.h"
#include "Mochii/Platform/OpenGL/OpenGLContext.h"
#include "Mochii/Renderer/Renderer.h"
#include "mzpch.h"

namespace Mochii {
Scope<GraphicsContext> GraphicsContext::Create(void* window) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      MI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
  }

  MI_CORE_ASSERT(false, "Unknown RendererAPI!");
  return nullptr;
}
}  // namespace Mochii
