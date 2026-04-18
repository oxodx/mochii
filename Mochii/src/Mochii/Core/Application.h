#pragma once
#include "Core.h"
#include "Mochii/Core/LayerStack.h"
#include "Mochii/Core/Timestep.h"
#include "Mochii/Events/ApplicationEvent.h"
#include "Mochii/Events/Event.h"
#include "Mochii/ImGui/ImGuiLayer.h"
#include "Window.h"

namespace Mochii {
class Application {
 public:
  Application();
  virtual ~Application();

  void Run();

  void OnEvent(Event& e);

  void PushLayer(Layer* layer);
  void PushOverlay(Layer* layer);

  inline Window& GetWindow() { return *_Window; }

  inline static Application& Get() {
    MI_CORE_ASSERT(_Instance, "Application not initialized!");
    return *_Instance;
  }

 private:
  bool OnWindowClose(WindowCloseEvent& e);
  bool OnWindowResize(WindowResizeEvent& e);

  std::unique_ptr<Window> _Window;
  ImGuiLayer* _ImGuiLayer;
  bool _Running = true;
  bool _Minimized = false;
  LayerStack _LayerStack;

  float _LastFrameTime = 0.0f;

  static Application* _Instance;
};

Application* CreateApplication();
}  // namespace Mochii
