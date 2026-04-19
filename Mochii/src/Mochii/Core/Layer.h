#pragma once
#include "Mochii/Core/Core.h"
#include "Mochii/Core/Timestep.h"
#include "Mochii/Events/Event.h"

namespace Mochii {
class Layer {
 public:
  Layer(const std::string& name = "Layer");
  virtual ~Layer() = default;

  virtual void OnAttach() {}
  virtual void OnDetach() {}
  virtual void OnUpdate(Timestep ts) {}
  virtual void OnImGuiRender() {}
  virtual void OnEvent(Event& event) {}

  const std::string& GetName() const { return _DebugName; }

 protected:
  std::string _DebugName;
};
}  // namespace Mochii
