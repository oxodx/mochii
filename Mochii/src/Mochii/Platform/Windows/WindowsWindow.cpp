#include "Mochii/Platform/Windows/WindowsWindow.h"
#include <GLFW/glfw3.h>
#include "Mochii/Events/ApplicationEvent.h"
#include "Mochii/Events/KeyEvent.h"
#include "Mochii/Events/MouseEvent.h"
#include "Mochii/Platform/OpenGL/OpenGLContext.h"
#include "Mochii/Renderer/GraphicsContext.h"
#include "Mochii/Renderer/Renderer.h"
#include "mzpch.h"

namespace Mochii {
static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description) {
  MI_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

Scope<Window> Window::Create(const WindowProps& props) {
  return CreateScope<WindowsWindow>(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) {
  MI_PROFILE_FUNCTION();

  Init(props);
}

WindowsWindow::~WindowsWindow() {
  MI_PROFILE_FUNCTION();

  Shutdown();
}

void WindowsWindow::Init(const WindowProps& props) {
  MI_PROFILE_FUNCTION();

  _Data.Title = props.Title;
  _Data.Width = props.Width;
  _Data.Height = props.Height;

  MI_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width,
               props.Height);

  if (s_GLFWWindowCount == 0) {
    MI_PROFILE_FUNCTION();

    int success = glfwInit();
    MI_CORE_ASSERT(success, "Could not intialize GLFW!");
    glfwSetErrorCallback(GLFWErrorCallback);
  }

  {
    MI_PROFILE_SCOPE("glfwCreateWindow");
#if defined(MI_DEBUG)
    if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
      glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    _Window = glfwCreateWindow((int)props.Width, (int)props.Height,
                               _Data.Title.c_str(), nullptr, nullptr);
    MI_CORE_ASSERT(_Window, "Failed to create GLFW window!");
    if (_Window) ++s_GLFWWindowCount;
  }

  _Context = GraphicsContext::Create(_Window);
  _Context->Init();

  glfwSetWindowUserPointer(_Window, &_Data);
  SetVSync(true);

  glfwSetWindowSizeCallback(
      _Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
      });

  glfwSetWindowCloseCallback(_Window, [](GLFWwindow* window) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    WindowCloseEvent event;
    data.EventCallback(event);
  });

  glfwSetKeyCallback(_Window, [](GLFWwindow* window, int key, int scancode,
                                 int action, int mods) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(key, 0);
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(key);
        data.EventCallback(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressedEvent event(key, 1);
        data.EventCallback(event);
        break;
      }
    }
  });

  glfwSetCharCallback(_Window, [](GLFWwindow* window, unsigned int keycode) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    KeyTypedEvent event(keycode);
    data.EventCallback(event);
  });

  glfwSetMouseButtonCallback(
      _Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
          case GLFW_PRESS: {
            MouseButtonPressedEvent event(button);
            data.EventCallback(event);
            break;
          }
          case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(button);
            data.EventCallback(event);
            break;
          }
        }
      });

  glfwSetScrollCallback(
      _Window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data.EventCallback(event);
      });

  glfwSetCursorPosCallback(
      _Window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseMovedEvent event((float)xPos, (float)yPos);
        data.EventCallback(event);
      });
}

void WindowsWindow::Shutdown() {
  MI_PROFILE_FUNCTION();

  glfwDestroyWindow(_Window);
  --s_GLFWWindowCount;

  if (s_GLFWWindowCount == 0) {
    glfwTerminate();
  }
}

void WindowsWindow::OnUpdate() {
  MI_PROFILE_FUNCTION();

  glfwPollEvents();
  glfwSwapBuffers(_Window);
}

void WindowsWindow::SetVSync(bool enabled) {
  MI_PROFILE_FUNCTION();

  if (enabled)
    glfwSwapInterval(1);
  else
    glfwSwapInterval(0);

  _Data.VSync = enabled;
}

bool WindowsWindow::IsVSync() const { return _Data.VSync; }
}  // namespace Mochii
