#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Mochii/Events/ApplicationEvent.h"
#include "Window.h"
#include "Mochii/LayerStack.h"
#include "Mochii/ImGui/ImGuiLayer.h"
#include "Mochii/Renderer/Shader.h"
#include "Mochii/Renderer/Buffer.h"
#include "Mochii/Renderer/VertexArray.h"

namespace Mochii {
	class MOCHII_API Application {
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *_Window; }

		inline static Application& Get() { return *_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> _Window;
		ImGuiLayer* _ImGuiLayer;
		bool _Running = true;
		LayerStack _LayerStack;

		std::shared_ptr<Shader> _Shader;
		std::shared_ptr<VertexArray> _VertexArray;

		std::shared_ptr<Shader> _BlueShader;
		std::shared_ptr<VertexArray> _SquareVA;

		static Application* _Instance;
	};

	Application* CreateApplication();
}
