#pragma once
#include <cstdint>
#include "Mochii/Renderer/RendererAPI.h"

namespace Mochii {
class RenderCommand {
 public:
  static void Init() { _RendererAPI->Init(); }

  static void SetViewport(uint32_t x, uint32_t y, uint32_t width,
                                 uint32_t height) {
    _RendererAPI->SetViewport(x, y, width, height);
  }

  static void SetClearColor(const glm::vec4& color) {
    _RendererAPI->SetClearColor(color);
  }

  static void Clear() { _RendererAPI->Clear(); }

  static void DrawIndexed(const Ref<VertexArray>& vertexArray,
                                 uint32_t count = 0) {
    _RendererAPI->DrawIndexed(vertexArray, count);
  }

 private:
  static Scope<RendererAPI> _RendererAPI;
};
}  // namespace Mochii
