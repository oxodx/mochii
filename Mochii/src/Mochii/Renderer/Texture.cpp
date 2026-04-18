#include "Texture.h"
#include "Mochii/Platform/OpenGL/OpenGLTexture.h"
#include "Renderer.h"
#include "mzpch.h"

namespace Mochii {
Ref<Texture2D> Texture2D::Create(const std::string& path) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      MI_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLTexture2D>(path);
  }

  MI_CORE_ASSERT(false, "Unknown RendererAPI!");
  return nullptr;
}
}  // namespace Mochii
