#include "Mochii/Renderer/OrthographicCameraController.h"
#include "Mochii/Core/Input.h"
#include "Mochii/Core/KeyCodes.h"
#include "mzpch.h"

namespace Mochii {
OrthographicCameraController::OrthographicCameraController(float aspectRatio,
                                                           bool rotation)
    : _AspectRatio(aspectRatio),
      _Camera(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel,
              -_ZoomLevel, _ZoomLevel),
      _Rotation(rotation) {}

void OrthographicCameraController::OnUpdate(Timestep ts) {
  MI_PROFILE_FUNCTION();

  if (Input::IsKeyPressed(Key::A)) {
    _CameraPosition.x -=
        cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
    _CameraPosition.y -=
        sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
  } else if (Input::IsKeyPressed(Key::D)) {
    _CameraPosition.x +=
        cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
    _CameraPosition.y +=
        sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
  }

  if (Input::IsKeyPressed(Key::W)) {
    _CameraPosition.x +=
        -sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
    _CameraPosition.y +=
        cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
  } else if (Input::IsKeyPressed(Key::S)) {
    _CameraPosition.x -=
        -sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
    _CameraPosition.y -=
        cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
  }

  if (_Rotation) {
    if (Input::IsKeyPressed(Key::Q))
      _CameraRotation += _CameraRotationSpeed * ts;
    if (Input::IsKeyPressed(Key::E))
      _CameraRotation -= _CameraRotationSpeed * ts;

    if (_CameraRotation > 180.0f)
      _CameraRotation -= 360.0f;
    else if (_CameraRotation <= -180.0f)
      _CameraRotation += 360.0f;

    _Camera.SetRotation(_CameraRotation);
  }

  _Camera.SetPosition(_CameraPosition);

  _CameraTranslationSpeed = _ZoomLevel;
}

void OrthographicCameraController::OnEvent(Event& e) {
  MI_PROFILE_FUNCTION();

  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<MouseScrolledEvent>(
      MI_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
  dispatcher.Dispatch<WindowResizeEvent>(
      MI_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
}

void OrthographicCameraController::OnResize(float width, float height) {
  _AspectRatio = width / height;
  _Camera.SetProjection(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel,
                        -_ZoomLevel, _ZoomLevel);
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
  MI_PROFILE_FUNCTION();

  _ZoomLevel -= e.GetYOffset() * 0.5f;
  _ZoomLevel = std::max(_ZoomLevel, 0.25f);
  SetZoomLevel(_ZoomLevel);
  return false;
}

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
  MI_PROFILE_FUNCTION();

  if (e.GetHeight() == 0) return false;
  OnResize((float)e.GetWidth(), (float)e.GetHeight());
  return false;
}
}  // namespace Mochii
