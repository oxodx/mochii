#pragma once
#include "Application.h"

#ifdef MI_PLATFORM_WINDOWS

extern Mochii::Application* Mochii::CreateApplication();

int main(int argc, char** argv) {
	auto app = Mochii::CreateApplication();
	app->Run();
	delete app;
}

#endif
