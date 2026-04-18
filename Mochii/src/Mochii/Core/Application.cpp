#include "Application.h"
#include <glfw/glfw3.h>
#include "Input.h"
#include "Mochii/Core/Application.h"
#include "Mochii/Core/Log.h"
#include "Mochii/Renderer/Renderer.h"
#include "mzpch.h"

namespace Mochii {
Application* Application::_Instance = nullptr;

Application::Application() {
  MI_CORE_ASSERT(!_Instance, "Application already exists!");
  _Instance = this;

  _Window = Window::Create();
  _Window->SetEventCallback(MI_BIND_EVENT_FN(Application::OnEvent));

  Renderer::Init();

  _ImGuiLayer = new ImGuiLayer();
  PushOverlay(_ImGuiLayer);
}

Application::~Application() {
  Renderer::Shutdown();
}

void Application::PushLayer(Layer* layer) { _LayerStack.PushLayer(layer); }

void Application::PushOverlay(Layer* layer) { _LayerStack.PushOverlay(layer); }

void Application::OnEvent(Event& e) {
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<WindowCloseEvent>(MI_BIND_EVENT_FN(Application::OnWindowClose));
  dispatcher.Dispatch<WindowResizeEvent>(MI_BIND_EVENT_FN(Application::OnWindowResize));

  for (auto it = _LayerStack.end(); it != _LayerStack.begin();) {
    (*--it)->OnEvent(e);
    if (e.Handled) break;
  }
}

void Application::Run() {
  _LastFrameTime = (float)glfwGetTime();
  while (_Running) {
    float time = (float)glfwGetTime();
    Timestep timestep = time - _LastFrameTime;
    _LastFrameTime = time;

    if (!_Minimized) {
      for (Layer* layer : _LayerStack) layer->OnUpdate(timestep);
    }

    _ImGuiLayer->Begin();
    for (Layer* layer : _LayerStack) layer->OnImGuiRender();
    _ImGuiLayer->End();

    _Window->OnUpdate();
  }
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
  _Running = false;
  return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e) {
  if (e.GetWidth() == 0 || e.GetHeight() == 0) {
    _Minimized = true;
    return false;
  }

  _Minimized = false;
  Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

  return false;
}
}  // namespace Mochii
