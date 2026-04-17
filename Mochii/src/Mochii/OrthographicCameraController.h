#pragma once
#include "Mochii/Renderer/OrthographicCamera.h"
#include "Mochii/Core/Timestep.h"
#include "Mochii/Events/ApplicationEvent.h"
#include "Mochii/Events/MouseEvent.h"

namespace Mochii {
	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return _Camera; }
		const OrthographicCamera& GetCamera() const { return _Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float _AspectRatio;
		float _ZoomLevel = 1.0f;
		OrthographicCamera _Camera;

		bool _Rotation;

		glm::vec3 _CameraPosition = { 0.0f, 0.0f, 0.0f };
		float _CameraRotation = 0.0f;
		float _CameraTranslationSpeed = 5.0f, _CameraRotationSpeed = 180.0f;
	};
}
