#include "Sandbox2D.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach() {
  MI_PROFILE_FUNCTION();
  InitializeGalaxy();
  InitializePlanets();
}

void Sandbox2D::OnDetach() { MI_PROFILE_FUNCTION(); }

void Sandbox2D::InitializeGalaxy() {
  m_Particles.clear();
  m_Particles.reserve(NumParticles);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159265f);
  std::uniform_real_distribution<float> radiusDist(0.1f, 1.0f);
  std::uniform_real_distribution<float> massDist(0.5f, 2.0f);

  for (int i = 0; i < NumParticles; ++i) {
    Particle p;

    float r = radiusDist(gen);

    p.distanceFromCenter = r * GalaxyRadius;
    p.angle = angleDist(gen);

    float orbitalSpeed = std::sqrt(1.0f / (p.distanceFromCenter + 0.1f)) * 0.3f;
    p.orbitalSpeed = orbitalSpeed;

    p.position.x = p.distanceFromCenter * std::cos(p.angle);
    p.position.y = p.distanceFromCenter * std::sin(p.angle);

    p.velocity.x = -orbitalSpeed * std::sin(p.angle);
    p.velocity.y = orbitalSpeed * std::cos(p.angle);

    p.mass = massDist(gen);

    float normalizedR = r;
    if (normalizedR < 0.15f) {
      float t = normalizedR / 0.15f;
      p.color = glm::vec4(1.0f, 0.9f - 0.1f * t, 0.7f - 0.3f * t, 1.0f);
    } else if (normalizedR < 0.4f) {
      p.color = glm::vec4(1.0f, 0.8f, 0.4f, 0.9f);
    } else if (normalizedR < 0.7f) {
      p.color = glm::vec4(0.7f, 0.8f, 1.0f, 0.8f);
    } else {
      p.color = glm::vec4(0.4f, 0.6f, 1.0f, 0.6f);
    }

    m_Particles.push_back(p);
  }
}

void Sandbox2D::InitializePlanets() {
  m_Planets.clear();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159265f);

  struct PlanetDef {
    float orbitRadius;
    float size;
    glm::vec4 color;
    const char* name;
  };

  std::vector<PlanetDef> definitions = {
    {0.3f, 0.04f, {0.6f, 0.6f, 0.6f, 1.0f}, "Mercury"},
    {0.45f, 0.06f, {0.9f, 0.7f, 0.4f, 1.0f}, "Venus"},
    {0.6f, 0.07f, {0.3f, 0.5f, 0.9f, 1.0f}, "Earth"},
    {0.8f, 0.05f, {0.9f, 0.3f, 0.2f, 1.0f}, "Mars"},
    {1.2f, 0.18f, {0.95f, 0.85f, 0.7f, 0.8f}, "Jupiter"},
    {1.6f, 0.15f, {0.9f, 0.85f, 0.6f, 0.8f}, "Saturn"},
    {2.0f, 0.1f, {0.5f, 0.7f, 0.9f, 0.8f}, "Uranus"},
    {2.5f, 0.1f, {0.3f, 0.4f, 0.8f, 0.8f}, "Neptune"},
  };

  for (size_t i = 0; i < definitions.size(); ++i) {
    const auto& def = definitions[i];
    Planet p;
    p.orbitRadius = def.orbitRadius;
    p.size = def.size;
    p.color = def.color;
    p.orbitSpeed = std::sqrt(1.0f / (p.orbitRadius + 0.1f)) * 0.5f;
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159265f);
    p.angle = angleDist(gen);
    p.selfRotation = 0.0f;
    p.position.x = p.orbitRadius * std::cos(p.angle);
    p.position.y = p.orbitRadius * std::sin(p.angle);
    m_Planets.push_back(p);
  }
}

void Sandbox2D::UpdateParticles(Mochii::Timestep ts) {
  float dt = ts * m_SimulationSpeed;
  float centralMass = 10.0f;

  for (auto& p : m_Particles) {
    glm::vec2 toCenter = -p.position;
    float dist = glm::length(toCenter);
    if (dist < 0.01f) dist = 0.01f;

    float forceMag = centralMass / (dist * dist);
    glm::vec2 acceleration = glm::normalize(toCenter) * forceMag;

    p.velocity += acceleration * dt;

    p.velocity *= 0.999f;

    p.position += p.velocity * dt;

    p.angle = std::atan2(p.position.y, p.position.x);
    p.distanceFromCenter = dist;
  }
}

void Sandbox2D::UpdatePlanets(Mochii::Timestep ts) {
  float dt = ts * m_SimulationSpeed;

  for (auto& p : m_Planets) {
    p.angle += p.orbitSpeed * dt;
    p.selfRotation += dt * 2.0f;
    p.position.x = p.orbitRadius * std::cos(p.angle);
    p.position.y = p.orbitRadius * std::sin(p.angle);
  }
}

void Sandbox2D::OnUpdate(Mochii::Timestep ts) {
  MI_PROFILE_FUNCTION();

  m_CameraController.OnUpdate(ts);
  UpdateParticles(ts);
  UpdatePlanets(ts);

  {
    MI_PROFILE_SCOPE("Renderer Prep");
    Mochii::RenderCommand::SetClearColor({0.02f, 0.02f, 0.05f, 1});
    Mochii::RenderCommand::Clear();
  }

  MI_PROFILE_SCOPE("Renderer Draw");
  Mochii::Renderer2D::BeginScene(m_CameraController.GetCamera());

  for (const auto& p : m_Particles) {
    float size = 0.02f + p.mass * 0.01f;
    size *= std::clamp(p.distanceFromCenter / GalaxyRadius, 0.5f, 1.5f);
    Mochii::Renderer2D::DrawQuad(p.position, {size, size}, p.color);
  }

  for (const auto& p : m_Planets) {
    Mochii::Renderer2D::DrawRotatedQuad(p.position, {p.size, p.size},
                                        p.selfRotation * 10.0f, p.color);
  }

  Mochii::Renderer2D::DrawQuad({0.0f, 0.0f}, {0.15f, 0.15f},
                              {1.0f, 0.95f, 0.8f, 1.0f});

  Mochii::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender() {
  MI_PROFILE_FUNCTION();

  static bool init = false;
  if (!init) {
    init = true;
    ImGui::GetIO().ConfigDockingWithShift = false;
  }

  if (ImGui::Begin("Controls")) {
    ImGui::SliderFloat("Simulation Speed", &m_SimulationSpeed, 0.0f, 5.0f);

    ImGui::Separator();
    ImGui::Text("Stars: %d", NumParticles);
    ImGui::Text("Planets: %d", (int)m_Planets.size());

    ImGui::Separator();
    ImGui::Text("Planets:");
    static const char* planetNames[] = {"Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune"};
    for (int i = 0; i < (int)m_Planets.size(); ++i) {
      bool selected = (m_SelectedPlanet == i);
      if (ImGui::Selectable(planetNames[i], selected)) {
        m_SelectedPlanet = i;
      }
    }

    if (m_SelectedPlanet >= 0 && m_SelectedPlanet < (int)m_Planets.size()) {
      ImGui::Separator();
      ImGui::Text("Selected: %s", planetNames[m_SelectedPlanet]);
      auto& p = m_Planets[m_SelectedPlanet];
      ImGui::Text("Orbit: %.2f, Speed: %.2f", p.orbitRadius, p.orbitSpeed);
      ImGui::ColorEdit4("Color", glm::value_ptr(p.color));
    }
    ImGui::End();
  }

  if (ImGui::Begin("Renderer")) {
    auto stats = Mochii::Renderer2D::GetStats();
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::End();
  }
}

void Sandbox2D::OnEvent(Mochii::Event& e) { m_CameraController.OnEvent(e); }
