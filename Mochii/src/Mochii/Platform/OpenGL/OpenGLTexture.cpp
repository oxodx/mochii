#include "mzpch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"
#include <glad/glad.h>

namespace Mochii {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
			: _Path(path) {
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

		MI_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &_RendererID);
		glTextureStorage2D(_RendererID, 1, internalFormat, _Width, _Height);

		glTextureParameteri(_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(_RendererID, 0, 0, 0, _Width, _Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, _RendererID);
	}
}
