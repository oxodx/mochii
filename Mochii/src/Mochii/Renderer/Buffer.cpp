#include "Buffer.h"
#include "Mochii/Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer.h"
#include "mzpch.h"

namespace Mochii {
VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      MI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return new OpenGLVertexBuffer(vertices, size);
  }

  MI_CORE_ASSERT(false, "Unknown RendererAPI!");
  return nullptr;
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      MI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return new OpenGLIndexBuffer(indices, size);
  }

  MI_CORE_ASSERT(false, "Unknown RendererAPI!");
  return nullptr;
}
}  // namespace Mochii
