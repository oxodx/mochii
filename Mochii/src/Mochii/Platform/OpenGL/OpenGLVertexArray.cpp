#include "OpenGLVertexArray.h"
#include <glad/glad.h>
#include "mzpch.h"

namespace Mochii {
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
  switch (type) {
    case Mochii::ShaderDataType::Float:
      return GL_FLOAT;
    case Mochii::ShaderDataType::Float2:
      return GL_FLOAT;
    case Mochii::ShaderDataType::Float3:
      return GL_FLOAT;
    case Mochii::ShaderDataType::Float4:
      return GL_FLOAT;
    case Mochii::ShaderDataType::Mat3:
      return GL_FLOAT;
    case Mochii::ShaderDataType::Mat4:
      return GL_FLOAT;
    case Mochii::ShaderDataType::Int:
      return GL_INT;
    case Mochii::ShaderDataType::Int2:
      return GL_INT;
    case Mochii::ShaderDataType::Int3:
      return GL_INT;
    case Mochii::ShaderDataType::Int4:
      return GL_INT;
    case Mochii::ShaderDataType::Bool:
      return GL_BOOL;
  }

  MI_CORE_ASSERT(false, "Unknown ShaderDataType!");
  return 0;
}

OpenGLVertexArray::OpenGLVertexArray() {
  glCreateVertexArrays(1, &_RendererID);
}

OpenGLVertexArray::~OpenGLVertexArray() {
  glDeleteVertexArrays(1, &_RendererID);
}

void OpenGLVertexArray::Bind() const { glBindVertexArray(_RendererID); }

void OpenGLVertexArray::Unbind() const { glBindVertexArray(0); }

void OpenGLVertexArray::AddVertexBuffer(
    const std::shared_ptr<VertexBuffer>& vertexBuffer) {
  MI_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(),
                 "Vertex Buffer has no layout!");

  glBindVertexArray(_RendererID);
  vertexBuffer->Bind();

  const auto& layout = vertexBuffer->GetLayout();
  for (const auto& element : layout) {
    glEnableVertexAttribArray(_VertexBufferIndex);
    glVertexAttribPointer(_VertexBufferIndex, element.GetComponentCount(),
                          ShaderDataTypeToOpenGLBaseType(element.Type),
                          element.Normalized ? GL_TRUE : GL_FALSE,
                          layout.GetStride(),
                          (const void*)element.Offset);
    _VertexBufferIndex++;
  }

  _VertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(
    const std::shared_ptr<IndexBuffer>& indexBuffer) {
  glBindVertexArray(_RendererID);
  indexBuffer->Bind();

  _IndexBuffer = indexBuffer;
}
}  // namespace Mochii
