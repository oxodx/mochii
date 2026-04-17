#include <Mochii.h>

class ExampleLayer : public Mochii::Layer {
public:
	ExampleLayer()
		: Layer("Example") { }

	void OnUpdate() override {
		MI_INFO("ExampleLayer::Update");
	}

	void OnEvent(Mochii::Event& event) override {
		MI_TRACE("{}", event.ToString());
	}
};

class Sandbox : public Mochii::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Mochii::ImGuiLayer());
	}

	~Sandbox() {

	}
};

Mochii::Application* Mochii::CreateApplication() {
	return new Sandbox();
}
