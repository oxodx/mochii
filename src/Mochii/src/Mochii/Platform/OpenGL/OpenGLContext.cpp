#include "Mochii/Platform/OpenGL/OpenGLContext.h"
#include <GLFW/glfw3.h>
#include "mzpch.h"

namespace Mochii {
OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
    : _WindowHandle(windowHandle) {
  MI_CORE_ASSERT(windowHandle, "Window handle is null!")
}

void OpenGLContext::Init() {
  MI_PROFILE_FUNCTION();

  glfwMakeContextCurrent(_WindowHandle);
  int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  MI_CORE_ASSERT(status, "Failed to initialize Glad!");

  MI_CORE_INFO("OpenGL Info:");
  MI_CORE_INFO("  Vendor: {}",
               reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
  MI_CORE_INFO("  Renderer: {}",
               reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
  MI_CORE_INFO("  Version: {}",
               reinterpret_cast<const char*>(glGetString(GL_VERSION)));

  MI_CORE_ASSERT(
      GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5),
      "Hazel requires at least OpenGL version 4.5!");
}

void OpenGLContext::SwapBuffers() {
  MI_PROFILE_FUNCTION();

  glfwSwapBuffers(_WindowHandle);
}
}  // namespace Mochii
