#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Mochii/Events/ApplicationEvent.h"
#include "Window.h"

namespace Mochii {
	class MOCHII_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> _Window;
		bool _Running = true;
	};

	Application* CreateApplication();
}
