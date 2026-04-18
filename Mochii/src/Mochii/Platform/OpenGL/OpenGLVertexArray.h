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

  virtual void AddVertexBuffer(
      const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
  virtual void SetIndexBuffer(
      const std::shared_ptr<IndexBuffer>& indexBuffer) override;

  virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers()
      const {
    return _VertexBuffers;
  }
  virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const {
    return _IndexBuffer;
  }

 private:
  uint32_t _RendererID;
  uint32_t _VertexBufferIndex = 0;
  std::vector<std::shared_ptr<VertexBuffer>> _VertexBuffers;
  std::shared_ptr<IndexBuffer> _IndexBuffer;
};
}  // namespace Mochii
