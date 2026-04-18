#include "Mochii/Platform/OpenGL/OpenGLTexture.h"
#include <glad/glad.h>
#include <stb_image.h>
#include "mzpch.h"

namespace Mochii {
OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    : _Width(width), _Height(height) {
  m_InternalFormat = GL_RGBA8;
  m_DataFormat = GL_RGBA;

  glCreateTextures(GL_TEXTURE_2D, 1, &_RendererID);
  glTextureStorage2D(_RendererID, 1, m_InternalFormat, _Width, _Height);

  glTextureParameteri(_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureParameteri(_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : _Path(path) {
  int width, height, channels;
  stbi_set_flip_vertically_on_load(1);
  stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  MI_CORE_ASSERT(data, "Failed to load image!");
  _Width = width;
  _Height = height;

  GLenum internalFormat = 0, dataFormat = 0;
  if (channels == 4) {
    internalFormat = GL_RGBA8;
    dataFormat = GL_RGBA;
  } else if (channels == 3) {
    internalFormat = GL_RGB8;
    dataFormat = GL_RGB;
  }

  m_InternalFormat = internalFormat;
  m_DataFormat = dataFormat;

  MI_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

  glCreateTextures(GL_TEXTURE_2D, 1, &_RendererID);
  glTextureStorage2D(_RendererID, 1, internalFormat, _Width, _Height);

  glTextureParameteri(_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureParameteri(_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTextureSubImage2D(_RendererID, 0, 0, 0, _Width, _Height, dataFormat,
                      GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &_RendererID); }

void OpenGLTexture2D::SetData(void* data, uint32_t size) {
  uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
  MI_CORE_ASSERT(size == _Width * _Height * bpp,
                 "Data must be entire texture!");
  glTextureSubImage2D(_RendererID, 0, 0, 0, _Width, _Height, m_DataFormat,
                      GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
  glBindTextureUnit(slot, _RendererID);
}
}  // namespace Mochii
