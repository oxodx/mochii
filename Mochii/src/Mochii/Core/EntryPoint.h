#pragma once
#include "Application.h"
#include "Log.h"

#ifdef MI_PLATFORM_WINDOWS

extern Mochii::Application* Mochii::CreateApplication();

int main(int argc, char** argv) {
  Mochii::Log::Init();
  std::unique_ptr<Mochii::Application> app{Mochii::CreateApplication()};
  if (!app) {
    MI_CORE_ERROR("Failed to create application!");
    return 1;
  }
  try {
    app->Run();
  } catch (const std::exception& e) {
    MI_CORE_ERROR("Application error: {0}", e.what());
    return 1;
  }
  return 0;
}

#endif
