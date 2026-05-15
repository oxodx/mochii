#include "UniformBuffer.h"
#include "Mochii/Renderer/Renderer.h"
#include "Mochii/Platform/OpenGL/OpenGLUniformBuffer.h"
#include "mzpch.h"

namespace Mochii {
Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      MI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLUniformBuffer>(size, binding);
  }

  MI_CORE_ASSERT(false, "Unknown RendererAPI!");
  return nullptr;
}
}  // namespace Mochii