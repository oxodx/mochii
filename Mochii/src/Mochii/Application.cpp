#include "mzpch.h"
#include "Application.h"
#include "Mochii/Log.h"
#include "Input.h"
#include "Mochii/Renderer/Renderer.h"
#include <glfw/glfw3.h>

namespace Mochii {
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::_Instance = nullptr;

	Application::Application() {
		MI_CORE_ASSERT(!_Instance, "Application already exists!");
		_Instance = this;

		_Window = std::unique_ptr<Window>(Window::Create());
		_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		_ImGuiLayer = new ImGuiLayer();
		PushOverlay(_ImGuiLayer);
	}

	void Application::PushLayer(Layer* layer) {
		_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer) {
		_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = _LayerStack.end(); it != _LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() {
		while (_Running) {
			float time = (float)glfwGetTime();
			Timestep timestep = time - _LastFrameTime;
			_LastFrameTime = time;

			for (Layer* layer : _LayerStack)
				layer->OnUpdate(timestep);

			_ImGuiLayer->Begin();
			for (Layer* layer : _LayerStack)
				layer->OnImGuiRender();
			_ImGuiLayer->End();

			_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		_Running = false;
		return true;
	}
}
