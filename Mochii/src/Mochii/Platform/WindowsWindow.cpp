#include "mzpch.h"
#include "WindowsWindow.h"

namespace Mochii {
	static bool _GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		_Data.Title = props.Title;
		_Data.Width = props.Width;
		_Data.Height = props.Height;

		MI_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!_GLFWInitialized) {
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			MI_CORE_ASSERT(success, "Could not intialize GLFW!");

			_GLFWInitialized = true;
		}

		_Window = glfwCreateWindow((int)props.Width, (int)props.Height, _Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(_Window);
		glfwSetWindowUserPointer(_Window, &_Data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(_Window);
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(_Window);
	}

	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const {
		return _Data.VSync;
	}
}
