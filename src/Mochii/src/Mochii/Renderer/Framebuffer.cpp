#include "Mochii/Renderer/Framebuffer.h"
#include "Mochii/Platform/OpenGL/OpenGLFramebuffer.h"
#include "Mochii/Renderer/Renderer.h"
#include "mzpch.h"

namespace Mochii {
Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      MI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLFramebuffer>(spec);
  }

  MI_CORE_ASSERT(false, "Unknown RendererAPI!");
  return nullptr;
}
}  // namespace Mochii
