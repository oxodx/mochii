#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Mochii/Events/ApplicationEvent.h"
#include "Window.h"
#include "Mochii/LayerStack.h"
#include "Mochii/Events/Event.h"
#include "Mochii/Events/ApplicationEvent.h"

namespace Mochii {
	class MOCHII_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> _Window;
		bool _Running = true;
		LayerStack _LayerStack;
	};

	Application* CreateApplication();
}
