#pragma once
#include "Mochii/Renderer/VertexArray.h"
#include "mzpch.h"

namespace Mochii {
class OpenGLVertexArray : public VertexArray {
 public:
  OpenGLVertexArray();
  virtual ~OpenGLVertexArray();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
  virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

  virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const {
    return _VertexBuffers;
  }
  virtual const Ref<IndexBuffer>& GetIndexBuffer() const {
    return _IndexBuffer;
  }

 private:
  uint32_t _RendererID;
  uint32_t _VertexBufferIndex = 0;
  std::vector<Ref<VertexBuffer>> _VertexBuffers;
  Ref<IndexBuffer> _IndexBuffer;
};
}  // namespace Mochii
