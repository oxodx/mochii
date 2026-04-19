#include "mzpch.h"
#include "Mochii/Core/Window.h"
#ifdef MI_PLATFORM_WINDOWS
#include "Mochii/Platform/Windows/WindowsWindow.h"
#endif

namespace Mochii {
Scope<Window> Window::Create(const WindowProps& props) {
#ifdef MI_PLATFORM_WINDOWS
  return CreateScope<WindowsWindow>(props);
#else
  MI_CORE_ASSERT(false, "Unknown platform!");
  return nullptr;
#endif
}
}  // namespace Mochii
