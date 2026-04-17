#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Mochii/Events/ApplicationEvent.h"
#include "Window.h"
#include "Mochii/LayerStack.h"
#include "Mochii/Events/Event.h"
#include "Mochii/Events/ApplicationEvent.h"
#include "Mochii/ImGui/ImGuiLayer.h"
#include "Mochii/Renderer/Shader.h"
#include "Mochii/Renderer/Buffer.h"

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

		unsigned int _VertexArray;
		std::unique_ptr<Shader> _Shader;
		std::unique_ptr<VertexBuffer> _VertexBuffer;
		std::unique_ptr<IndexBuffer> _IndexBuffer;

		static Application* _Instance;
	};

	Application* CreateApplication();
}
