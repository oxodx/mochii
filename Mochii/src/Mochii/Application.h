#pragma once
#include "Core.h"

namespace Mochii {
	class MOCHII_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}
