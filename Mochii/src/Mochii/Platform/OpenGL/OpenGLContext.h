#pragma once
#include "Mochii/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Mochii {
	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* _WindowHandle;
	};
}
