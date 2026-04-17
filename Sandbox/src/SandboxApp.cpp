#include <Mochii.h>
#include "Mochii/Platform/OpenGL/OpenGLShader.h"
#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Mochii::Layer {
public:
	ExampleLayer()
			: Layer("Example"), _Camera(-1.6f, 1.6f, -0.9f, 0.9f), _CameraPosition(0.0f) {
		_VertexArray.reset(Mochii::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Mochii::Ref<Mochii::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Mochii::VertexBuffer::Create(vertices, sizeof(vertices)));
		Mochii::BufferLayout layout = {
			{ Mochii::ShaderDataType::Float3, "a_Position" },
			{ Mochii::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Mochii::Ref<Mochii::IndexBuffer> indexBuffer;
		indexBuffer.reset(Mochii::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		_VertexArray->SetIndexBuffer(indexBuffer);

		_SquareVA.reset(Mochii::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Mochii::Ref<Mochii::VertexBuffer> squareVB;
		squareVB.reset(Mochii::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Mochii::ShaderDataType::Float3, "a_Position" }
			});
		_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Mochii::Ref<Mochii::IndexBuffer> squareIB;
		squareIB.reset(Mochii::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		_Shader.reset(Mochii::Shader::Create(vertexSrc, fragmentSrc));

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main() {
				color = vec4(u_Color, 1.0);
			}
		)";

		_FlatColorShader.reset(Mochii::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));
	}

	void OnUpdate(Mochii::Timestep ts) override {
		if (Mochii::Input::IsKeyPressed(MI_KEY_LEFT))
			_CameraPosition.x -= _CameraMoveSpeed * ts;
		else if (Mochii::Input::IsKeyPressed(MI_KEY_RIGHT))
			_CameraPosition.x += _CameraMoveSpeed * ts;

		if (Mochii::Input::IsKeyPressed(MI_KEY_UP))
			_CameraPosition.y += _CameraMoveSpeed * ts;
		else if (Mochii::Input::IsKeyPressed(MI_KEY_DOWN))
			_CameraPosition.y -= _CameraMoveSpeed * ts;

		if (Mochii::Input::IsKeyPressed(MI_KEY_A))
			_CameraRotation += _CameraRotationSpeed * ts;
		if (Mochii::Input::IsKeyPressed(MI_KEY_D))
			_CameraRotation -= _CameraRotationSpeed * ts;

		Mochii::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Mochii::RenderCommand::Clear();

		_Camera.SetPosition(_CameraPosition);
		_Camera.SetRotation(_CameraRotation);

		Mochii::Renderer::BeginScene(_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Mochii::OpenGLShader>(_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Mochii::OpenGLShader>(_FlatColorShader)->UploadUniformFloat3("u_Color", _SquareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Mochii::Renderer::Submit(_FlatColorShader, _SquareVA, transform);
			}
		}

		Mochii::Renderer::Submit(_Shader, _VertexArray);

		Mochii::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(_SquareColor));
		ImGui::End();
	}

	void OnEvent(Mochii::Event& event) override {
	
	}
private:
	std::shared_ptr<Mochii::Shader> _Shader;
	std::shared_ptr<Mochii::VertexArray> _VertexArray;

	std::shared_ptr<Mochii::Shader> _FlatColorShader;
	std::shared_ptr<Mochii::VertexArray> _SquareVA;

	Mochii::OrthographicCamera _Camera;
	glm::vec3 _CameraPosition;
	float _CameraMoveSpeed = 5.0f;

	float _CameraRotation = 0.0f;
	float _CameraRotationSpeed = 180.0f;

	glm::vec3 _SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Mochii::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Mochii::Application* Mochii::CreateApplication() {
	return new Sandbox();
}
