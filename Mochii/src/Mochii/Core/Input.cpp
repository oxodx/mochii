#include "mzpch.h"
#include "Mochii/Core/Input.h"
#ifdef MI_PLATFORM_WINDOWS
#include "Mochii/Platform/Windows/WindowsInput.h"
#endif

namespace Mochii {
Scope<Input> Input::_Instance = Input::Create();

Scope<Input> Input::Create() {
#ifdef MI_PLATFORM_WINDOWS
  return CreateScope<WindowsInput>();
#else
  MI_CORE_ASSERT(false, "Unknown platform!");
  return nullptr;
#endif
}
}  // namespace Mochii
