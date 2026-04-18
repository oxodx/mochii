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

#ifdef MI_ENABLE_ASSERTS
  int versionMajor;
  int versionMinor;
  glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
  glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

  MI_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5),
                 "Mochii requires at least OpenGL version 4.5!");
#endif
}

void OpenGLContext::SwapBuffers() {
  MI_PROFILE_FUNCTION();

  glfwSwapBuffers(_WindowHandle);
}
}  // namespace Mochii
