#pragma once
#include "Mochii/Core/Base.h"

namespace Mochii {
class Input {
 public:
  static bool IsKeyPressed(int key);

  static bool IsMouseButtonPressed(int button);
  static std::pair<float, float> GetMousePosition();
  static float GetMouseX();
  static float GetMouseY();
};
}  // namespace Mochii
