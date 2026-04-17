#include "mzpch.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace Mochii {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: _WindowHandle(windowHandle) {
		MI_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MI_CORE_ASSERT(status, "Failed to initialize Glad!");

		MI_CORE_INFO("OpenGL Info:");
		MI_CORE_INFO("  Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		MI_CORE_INFO("  Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		MI_CORE_INFO("  Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(_WindowHandle);
	}
}
