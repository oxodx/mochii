#include <Mochii.h>
#include <Mochii/Core/EntryPoint.h>
#include "Sandbox2D.h"

class Sandbox : public Mochii::Application {
 public:
  Sandbox() { PushLayer(new Sandbox2D()); }

  ~Sandbox() {}
};

Mochii::Application* Mochii::CreateApplication() { return new Sandbox(); }
