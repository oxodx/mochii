#include <Mochii.h>
#include "imgui.h"

class ExampleLayer : public Mochii::Layer {
public:
	ExampleLayer()
		: Layer("Example") { }

	void OnUpdate() override {
		if (Mochii::Input::IsKeyPressed(MI_KEY_TAB))
			MI_TRACE("Tab key is pressed (poll)!");
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Mochii::Event& event) override {
		if (event.GetEventType() == Mochii::EventType::KeyPressed) {
			Mochii::KeyPressedEvent& e = (Mochii::KeyPressedEvent&)event;
			if (e.GetKeyCode() == MI_KEY_TAB)
				MI_TRACE("Tab key is pressed (event)!");
			MI_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Mochii::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Mochii::Application* Mochii::CreateApplication() {
	return new Sandbox();
}
