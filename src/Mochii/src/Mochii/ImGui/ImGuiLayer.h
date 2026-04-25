#pragma once
#include "Mochii/Core/Layer.h"
#include "Mochii/Events/ApplicationEvent.h"
#include "Mochii/Events/KeyEvent.h"
#include "Mochii/Events/MouseEvent.h"

namespace Mochii {
class ImGuiLayer : public Layer {
 public:
  ImGuiLayer();
  ~ImGuiLayer() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnEvent(Event& e) override;

  void Begin();
  void End();

  void BlockEvents(bool block) { m_BlockEvents = block; }

  void SetDarkThemeColors();

 private:
  bool m_BlockEvents = true;
  float _Time = 0.0f;
};
}  // namespace Mochii
