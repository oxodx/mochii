#pragma once
#include "Application.h"
#include "Log.h"

#ifdef MI_PLATFORM_WINDOWS

extern Mochii::Application* Mochii::CreateApplication();

int main(int argc, char** argv) {
	Mochii::Log::Init();
	MI_CORE_WARN("Initialized Log!");
	MI_INFO("Hello!");
	
	auto app = Mochii::CreateApplication();
	app->Run();
	delete app;
}

#endif
