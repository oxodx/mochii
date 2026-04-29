#pragma once
#include <functional>
#include "Mochii/Core/Base.h"
#include "Mochii/Debug/Instrumentor.h"

namespace Mochii {
enum class EventType {
  None = 0,
  WindowClose,
  WindowResize,
  WindowFocus,
  WindowLostFocus,
  WindowMoved,
  AppTick,
  AppUpdate,
  AppRender,
  KeyPressed,
  KeyReleased,
  KeyTyped,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled
};

enum class EventCategory : uint32_t {
  None = 0,
  Application = BIT(0),
  Input = BIT(1),
  Keyboard = BIT(2),
  Mouse = BIT(3),
  MouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                \
  static EventType GetStaticType() { return EventType::type; }                \
  virtual EventType GetEventType() const override { return GetStaticType(); } \
  virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
  virtual uint32_t GetCategoryFlags() const override { return static_cast<uint32_t>(category); }

class Event {
 public:
  virtual ~Event() = default;

  bool Handled = false;

  virtual EventType GetEventType() const = 0;
  virtual const char* GetName() const = 0;
  virtual uint32_t GetCategoryFlags() const = 0;
  virtual std::string ToString() const { return GetName(); }

  bool IsInCategory(EventCategory category) {
    return GetCategoryFlags() & static_cast<uint32_t>(category);
  }
};

class EventDispatcher {
 public:
  EventDispatcher(Event& event) : _Event(event) {}

  // F will be deduced by the compiler
  template <typename T, typename F>
  bool Dispatch(const F& func) {
    if (_Event.GetEventType() == T::GetStaticType()) {
      _Event.Handled |= func(static_cast<T&>(_Event));
      return true;
    }
    return false;
  }

 private:
  Event& _Event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) {
  return os << e.ToString();
}
}  // namespace Mochii
