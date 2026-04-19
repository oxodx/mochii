#pragma once
#include <Mochii.h>
#include <vector>
#include <cmath>

class Sandbox2D : public Mochii::Layer {
 public:
  Sandbox2D();
  virtual ~Sandbox2D() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;

  void OnUpdate(Mochii::Timestep ts) override;
  virtual void OnImGuiRender() override;
  void OnEvent(Mochii::Event& e) override;

private:
  Mochii::OrthographicCameraController m_CameraController;

  struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    float mass;
    glm::vec4 color;
    float distanceFromCenter;
    float angle;
    float orbitalSpeed;
  };

  struct Planet {
    glm::vec2 position;
    float orbitRadius;
    float orbitSpeed;
    float angle;
    float size;
    glm::vec4 color;
    float selfRotation;
  };

  std::vector<Particle> m_Particles;
  std::vector<Planet> m_Planets;
  static constexpr int NumParticles = 2000;
  static constexpr float GalaxyRadius = 5.0f;

  float m_SimulationSpeed = 1.0f;
  float m_CameraZoom = 1.0f;
  int m_SelectedPlanet = -1;

  void InitializeGalaxy();
  void InitializePlanets();
  void UpdateParticles(Mochii::Timestep ts);
  void UpdatePlanets(Mochii::Timestep ts);
};
