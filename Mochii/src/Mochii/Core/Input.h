#pragma once
#include "Mochii/Core/Core.h"

namespace Mochii {
class Input {
 protected:
  Input() = default;

 public:
  virtual ~Input() = default;
  Input(const Input&) = delete;
  Input& operator=(const Input&) = delete;

  static bool IsKeyPressed(int keycode) {
    MI_CORE_ASSERT(_Instance, "Input not initialized!");
    return _Instance->IsKeyPressedImpl(keycode);
  }

  static bool IsMouseButtonPressed(int button) {
    MI_CORE_ASSERT(_Instance, "Input not initialized!");
    return _Instance->IsMouseButtonPressedImpl(button);
  }
  static std::pair<float, float> GetMousePosition() {
    MI_CORE_ASSERT(_Instance, "Input not initialized!");
    return _Instance->GetMousePositionImpl();
  }
  static float GetMouseX() {
    MI_CORE_ASSERT(_Instance, "Input not initialized!");
    return _Instance->GetMouseXImpl();
  }
  static float GetMouseY() {
    MI_CORE_ASSERT(_Instance, "Input not initialized!");
    return _Instance->GetMouseYImpl();
  }

  static Scope<Input> Create();

 protected:
  virtual bool IsKeyPressedImpl(int keycode) = 0;

  virtual bool IsMouseButtonPressedImpl(int button) = 0;
  virtual std::pair<float, float> GetMousePositionImpl() = 0;
  virtual float GetMouseXImpl() = 0;
  virtual float GetMouseYImpl() = 0;

 private:
  static Scope<Input> _Instance;
};
}  // namespace Mochii
