#pragma once
#include "Mochii/Core/Core.h"

namespace Mochii {
	class MOCHII_API Input {
	protected:
		Input() = default;
		virtual ~Input() = default;
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool IsKeyPressed(int keycode) {
			MI_CORE_ASSERT(_Instance, "Input not initialized!");
			return _Instance->IsKeyPressedImpl(keycode);
		}

		inline static bool IsMouseButtonPressed(int button) {
			MI_CORE_ASSERT(_Instance, "Input not initialized!");
			return _Instance->IsMouseButtonPressedImpl(button);
		}
		inline static std::pair<float, float> GetMousePosition() {
			MI_CORE_ASSERT(_Instance, "Input not initialized!");
			return _Instance->GetMousePositionImpl();
		}
		inline static float GetMouseX() {
			MI_CORE_ASSERT(_Instance, "Input not initialized!");
			return _Instance->GetMouseXImpl();
		}
		inline static float GetMouseY() {
			MI_CORE_ASSERT(_Instance, "Input not initialized!");
			return _Instance->GetMouseYImpl();
		}
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Input* _Instance;
	};
}
