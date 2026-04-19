#include "Mochii/Platform/OpenGL/OpenGLBuffer.h"
#include <glad/glad.h>
#include "mzpch.h"

namespace Mochii {
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
  MI_PROFILE_FUNCTION();

  glCreateBuffers(1, &_RendererID);
  glBindBuffer(GL_ARRAY_BUFFER, _RendererID);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
  MI_PROFILE_FUNCTION();

  glCreateBuffers(1, &_RendererID);
  glBindBuffer(GL_ARRAY_BUFFER, _RendererID);
  glBufferData(GL_ARRAY_BUFFER, size, (const void*)vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
  MI_PROFILE_FUNCTION();

  glDeleteBuffers(1, &_RendererID);
}

void OpenGLVertexBuffer::Bind() const {
  MI_PROFILE_FUNCTION();

  glBindBuffer(GL_ARRAY_BUFFER, _RendererID);
}

void OpenGLVertexBuffer::Unbind() const {
  MI_PROFILE_FUNCTION();

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
  glBindBuffer(GL_ARRAY_BUFFER, _RendererID);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    : _Count(count) {
  MI_PROFILE_FUNCTION();

  glCreateBuffers(1, &_RendererID);
  glBindBuffer(GL_ARRAY_BUFFER, _RendererID);
  glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
  MI_PROFILE_FUNCTION();

  glDeleteBuffers(1, &_RendererID);
}

void OpenGLIndexBuffer::Bind() const {
  MI_PROFILE_FUNCTION();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererID);
}

void OpenGLIndexBuffer::Unbind() const {
  MI_PROFILE_FUNCTION();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
}  // namespace Mochii
