#pragma once
#include <glm/glm.hpp>
#include "Mochii/Core/KeyCodes.h"
#include "Mochii/Core/MouseCodes.h"

namespace Mochii {
class Input {
 public:
  static bool IsKeyPressed(KeyCode key);

  static bool IsMouseButtonPressed(MouseCode button);
  static glm::vec2 GetMousePosition();
  static float GetMouseX();
  static float GetMouseY();
};
}  // namespace Mochii
