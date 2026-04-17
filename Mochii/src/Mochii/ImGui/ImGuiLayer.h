#pragma once
#include "Mochii/Layer.h"
#include "Mochii/Events/ApplicationEvent.h"
#include "Mochii/Events/KeyEvent.h"
#include "Mochii/Events/MouseEvent.h"

namespace Mochii {
	class MOCHII_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float _Time = 0.0f;
	};
}
