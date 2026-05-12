#pragma once
#include "Mochii/Core/Assert.h"
#include "Mochii/Core/Base.h"
#include "Mochii/Core/LayerStack.h"
#include "Mochii/Core/Timestep.h"
#include "Mochii/Events/ApplicationEvent.h"
#include "Mochii/Events/Event.h"
#include "Mochii/ImGui/ImGuiLayer.h"
#include "Window.h"

int main(int argc, char** argv);

namespace Mochii {
struct ApplicationCommandLineArgs {
  int Count = 0;
  char** Args = nullptr;

  const char* operator[](int index) const {
    MI_CORE_ASSERT(index < Count);
    return Args[index];
  }
};

class Application {
 public:
  Application(const std::string& name = "Mochii App",
              ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
  virtual ~Application();

  void OnEvent(Event& e);

  void PushLayer(Layer* layer);
  void PushOverlay(Layer* layer);

  LayerStack& GetLayerStack() { return _LayerStack; }

  Window& GetWindow() { return *_Window; }

  void Close();

  ImGuiLayer* GetImGuiLayer() { return _ImGuiLayer; }

  virtual void OnMenuNewScene() {}
  virtual void OnMenuOpenScene() {}
  virtual void OnMenuSaveSceneAs() {}

  static Application& Get() {
    MI_CORE_ASSERT(_Instance, "Application not initialized!");
    return *_Instance;
  }

  ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }

 private:
  void Run();
  bool OnWindowClose(WindowCloseEvent& e);
  bool OnWindowResize(WindowResizeEvent& e);

  ApplicationCommandLineArgs m_CommandLineArgs;
  std::unique_ptr<Window> _Window;
  ImGuiLayer* _ImGuiLayer;
  bool _Running = true;
  bool _Minimized = false;
  LayerStack _LayerStack;

  float _LastFrameTime = 0.0f;

  static Application* _Instance;
  friend int ::main(int argc, char** argv);
};

Application* CreateApplication(ApplicationCommandLineArgs args);
}  // namespace Mochii
