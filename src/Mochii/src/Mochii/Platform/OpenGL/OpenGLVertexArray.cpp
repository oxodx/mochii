#include "Mochii/Platform/OpenGL/OpenGLVertexArray.h"
#include <glad/glad.h>
#include "mzpch.h"

namespace Mochii {
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::None:
      return 0;
    case ShaderDataType::Float:
      return GL_FLOAT;
    case ShaderDataType::Float2:
      return GL_FLOAT;
    case ShaderDataType::Float3:
      return GL_FLOAT;
    case ShaderDataType::Float4:
      return GL_FLOAT;
    case ShaderDataType::Mat3:
      return GL_FLOAT;
    case ShaderDataType::Mat4:
      return GL_FLOAT;
    case ShaderDataType::Int:
      return GL_INT;
    case ShaderDataType::Int2:
      return GL_INT;
    case ShaderDataType::Int3:
      return GL_INT;
    case ShaderDataType::Int4:
      return GL_INT;
    case ShaderDataType::Bool:
      return GL_BOOL;
  }

  MI_CORE_ASSERT(false, "Unknown ShaderDataType!");
  return 0;
}

OpenGLVertexArray::OpenGLVertexArray() {
  MI_PROFILE_FUNCTION();

  glCreateVertexArrays(1, &_RendererID);
}

OpenGLVertexArray::~OpenGLVertexArray() {
  MI_PROFILE_FUNCTION();

  glDeleteVertexArrays(1, &_RendererID);
}

void OpenGLVertexArray::Bind() const {
  MI_PROFILE_FUNCTION();

  glBindVertexArray(_RendererID);
}

void OpenGLVertexArray::Unbind() const {
  MI_PROFILE_FUNCTION();

  glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
  MI_PROFILE_FUNCTION();

  MI_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(),
                 "Vertex Buffer has no layout!");

  glBindVertexArray(_RendererID);
  vertexBuffer->Bind();

  const auto& layout = vertexBuffer->GetLayout();
  for (const auto& element : layout) {
    switch (element.Type) {
      case ShaderDataType::Float:
      case ShaderDataType::Float2:
      case ShaderDataType::Float3:
      case ShaderDataType::Float4: {
        glEnableVertexAttribArray(_VertexBufferIndex);
        glVertexAttribPointer(_VertexBufferIndex, element.GetComponentCount(),
                              ShaderDataTypeToOpenGLBaseType(element.Type),
                              element.Normalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(), (const void*)element.Offset);
        _VertexBufferIndex++;
        break;
      }
      case ShaderDataType::Int:
      case ShaderDataType::Int2:
      case ShaderDataType::Int3:
      case ShaderDataType::Int4:
      case ShaderDataType::Bool: {
        glEnableVertexAttribArray(_VertexBufferIndex);
        glVertexAttribIPointer(_VertexBufferIndex, element.GetComponentCount(),
                               ShaderDataTypeToOpenGLBaseType(element.Type),
                               layout.GetStride(), (const void*)element.Offset);
        _VertexBufferIndex++;
        break;
      }
      case ShaderDataType::Mat3:
      case ShaderDataType::Mat4: {
        uint8_t columnCount = (element.Type == ShaderDataType::Mat3) ? 3 : 4;
        for (uint8_t i = 0; i < columnCount; i++) {
          glEnableVertexAttribArray(_VertexBufferIndex);
          glVertexAttribPointer(
              _VertexBufferIndex, columnCount,
              ShaderDataTypeToOpenGLBaseType(element.Type),
              element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
              (const void*)(element.Offset + sizeof(float) * columnCount * i));
          glVertexAttribDivisor(_VertexBufferIndex, 1);
          _VertexBufferIndex++;
        }
        break;
      }
      default:
        MI_CORE_ASSERT(false, "Unknown ShaderDataType!");
    }
  }

  _VertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
  MI_PROFILE_FUNCTION();

  glBindVertexArray(_RendererID);
  indexBuffer->Bind();

  _IndexBuffer = indexBuffer;
}
}  // namespace Mochii
