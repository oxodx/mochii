#include "mzpch.h"
#include "Application.h"
#include "Mochii/Events/ApplicationEvent.h"
#include "Mochii/Log.h"

namespace Mochii {
	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication)) {
			MI_TRACE("{}", e.ToString());
		}
		if (e.IsInCategory(EventCategoryInput)) {
			MI_TRACE("{}", e.ToString());
		}

		while (true);
	}
}
