#include <Mochii.h>

class Sandbox : public Mochii::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

Mochii::Application* Mochii::CreateApplication() {
	return new Sandbox();
}
