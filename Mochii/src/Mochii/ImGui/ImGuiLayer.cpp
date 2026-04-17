#include "mzpch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "Mochii/Application.h"
#include <glad/glad.h>

namespace Mochii {
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer") { }

	ImGuiLayer::~ImGuiLayer() {

	}

	void ImGuiLayer::OnAttach() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

		GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate() {
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = _Time > 0.0f ? (time - _Time) : (1.0f / 60.0f);
		_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(MI_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(MI_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(MI_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(MI_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(MI_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(MI_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(MI_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(MI_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(e.GetMouseButton(), true);

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(e.GetMouseButton(), false);

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMousePosEvent(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent(e.GetXOffset(), e.GetYOffset());

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(ImGuiKey_LeftCtrl, true);
		io.AddKeyEvent(ImGuiKey_LeftShift, true);
		io.AddKeyEvent(ImGuiKey_LeftAlt, true);
		io.AddKeyEvent(ImGuiKey_LeftSuper, true);
		io.AddKeyEvent(ImGuiKey_RightCtrl, true);
		io.AddKeyEvent(ImGuiKey_RightShift, true);
		io.AddKeyEvent(ImGuiKey_RightAlt, true);
		io.AddKeyEvent(ImGuiKey_RightSuper, true);

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(ImGuiKey_LeftCtrl, false);
		io.AddKeyEvent(ImGuiKey_LeftShift, false);
		io.AddKeyEvent(ImGuiKey_LeftAlt, false);
		io.AddKeyEvent(ImGuiKey_LeftSuper, false);
		io.AddKeyEvent(ImGuiKey_RightCtrl, false);
		io.AddKeyEvent(ImGuiKey_RightShift, false);
		io.AddKeyEvent(ImGuiKey_RightAlt, false);
		io.AddKeyEvent(ImGuiKey_RightSuper, false);

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}
}
