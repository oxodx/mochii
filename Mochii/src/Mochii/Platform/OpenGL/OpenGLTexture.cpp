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

		glCreateTextures(GL_TEXTURE_2D, 1, &_RendererID);
		glTextureStorage2D(_RendererID, 1, GL_RGB8, _Width, _Height);

		glTextureParameteri(_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(_RendererID, 0, 0, 0, _Width, _Height, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, _RendererID);
	}
}
