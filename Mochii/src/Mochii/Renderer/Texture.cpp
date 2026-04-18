#include "Mochii/Renderer/Texture.h"
#include "Mochii/Platform/OpenGL/OpenGLTexture.h"
#include "Mochii/Renderer/Renderer.h"
#include "mzpch.h"

namespace Mochii {
Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      MI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLTexture2D>(width, height);
  }

  MI_CORE_ASSERT(false, "Unknown RendererAPI!");
  return nullptr;
}

Ref<Texture2D> Texture2D::Create(const std::string& path) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      MI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return CreateRef<OpenGLTexture2D>(path);
  }

  MI_CORE_ASSERT(false, "Unknown RendererAPI!");
  return nullptr;
}
}  // namespace Mochii
