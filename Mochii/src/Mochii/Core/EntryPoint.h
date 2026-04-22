#pragma once
#include "Mochii/Core/Application.h"
#include "Mochii/Core/Base.h"
#include "Mochii/Core/Log.h"

#ifdef MI_PLATFORM_WINDOWS

extern Mochii::Application* Mochii::CreateApplication();

int main(int argc, char** argv) {
  Mochii::Log::Init();
  MI_PROFILE_BEGIN_SESSION("Startup", "MochiiProfile-Startup.json");
  std::unique_ptr<Mochii::Application> app{Mochii::CreateApplication()};
  if (!app) {
    MI_CORE_ERROR("Failed to create application!");
    return 1;
  }
  MI_PROFILE_END_SESSION();

  MI_PROFILE_BEGIN_SESSION("Runtime", "MochiiProfile-Runtime.json");
  try {
    app->Run();
  } catch (const std::exception& e) {
    MI_CORE_ERROR("Application error: {0}", e.what());
    return 1;
  }
  MI_PROFILE_END_SESSION();

  return 0;
}

#endif
