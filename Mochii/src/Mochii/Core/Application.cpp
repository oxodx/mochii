#include "Application.h"
#include <glfw/glfw3.h>
#include "Input.h"
#include "Mochii/Core/Application.h"
#include "Mochii/Core/Log.h"
#include "Mochii/Renderer/Renderer.h"
#include "mzpch.h"

namespace Mochii {
Application* Application::_Instance = nullptr;

Application::Application(const std::string& name) {
  MI_PROFILE_FUNCTION();

  MI_CORE_ASSERT(!_Instance, "Application already exists!");
  _Instance = this;

  _Window = Window::Create(WindowProps(name));
  _Window->SetEventCallback(MI_BIND_EVENT_FN(Application::OnEvent));

  Renderer::Init();

  _ImGuiLayer = new ImGuiLayer();
  PushOverlay(_ImGuiLayer);
}

Application::~Application() {
  MI_PROFILE_FUNCTION();

  Renderer::Shutdown();
}

void Application::PushLayer(Layer* layer) {
  MI_PROFILE_FUNCTION();

  _LayerStack.PushLayer(layer);
  layer->OnAttach();
}

void Application::PushOverlay(Layer* layer) {
  MI_PROFILE_FUNCTION();

  _LayerStack.PushOverlay(layer);
  layer->OnAttach();
}

void Application::Close() { _Running = false; }

void Application::OnEvent(Event& e) {
  MI_PROFILE_FUNCTION();

  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<WindowCloseEvent>(
      MI_BIND_EVENT_FN(Application::OnWindowClose));
  dispatcher.Dispatch<WindowResizeEvent>(
      MI_BIND_EVENT_FN(Application::OnWindowResize));

  for (auto it = _LayerStack.rbegin(); it != _LayerStack.rend(); ++it) {
    if (e.Handled) break;
    (*it)->OnEvent(e);
  }
}

void Application::Run() {
  MI_PROFILE_FUNCTION();

  while (_Running) {
    MI_PROFILE_SCOPE("RunLoop");

    float time = (float)glfwGetTime();
    Timestep timestep = time - _LastFrameTime;
    _LastFrameTime = time;

    if (!_Minimized) {
      {
        MI_PROFILE_SCOPE("LayerStack OnUpdate");

        for (Layer* layer : _LayerStack) layer->OnUpdate(timestep);
      }

      _ImGuiLayer->Begin();
      {
        MI_PROFILE_SCOPE("LayerStack OnImGuiRender");

        for (Layer* layer : _LayerStack) layer->OnImGuiRender();
      }
      _ImGuiLayer->End();
    }

    _Window->OnUpdate();
  }
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
  _Running = false;
  return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e) {
  MI_PROFILE_FUNCTION();

  if (e.GetWidth() == 0 || e.GetHeight() == 0) {
    _Minimized = true;
    return false;
  }

  _Minimized = false;
  Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

  return false;
}
}  // namespace Mochii
