#pragma once
#include "Mochii/Layer.h"

namespace Mochii {
	class MOCHII_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		float _Time = 0.0f;
	};
}
