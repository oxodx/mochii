#include <Mochii.h>
#include "Mochii/Platform/OpenGL/OpenGLShader.h"
#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Mochii::Layer {
public:
	ExampleLayer()
			: Layer("Example"), _CameraController(1280.0f / 720.0f) {
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

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Mochii::Ref<Mochii::VertexBuffer> squareVB;
		squareVB.reset(Mochii::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Mochii::ShaderDataType::Float3, "a_Position" },
			{ Mochii::ShaderDataType::Float2, "a_TexCoord" }
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

		_Shader = Mochii::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

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

		_FlatColorShader = Mochii::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		auto textureShader = _ShaderLibrary.Load("assets/shaders/Texture.glsl");

		_Texture = Mochii::Texture2D::Create("assets/textures/Checkerboard.png");

		std::dynamic_pointer_cast<Mochii::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Mochii::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Mochii::Timestep ts) override {
		// Update
		_CameraController.OnUpdate(ts);

		// Render
		Mochii::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Mochii::RenderCommand::Clear();

		Mochii::Renderer::BeginScene(_CameraController.GetCamera());

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

		auto textureShader = _ShaderLibrary.Get("Texture");

		_Texture->Bind();
		Mochii::Renderer::Submit(textureShader, _SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		// Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Mochii::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(_SquareColor));
		ImGui::End();
	}

	void OnEvent(Mochii::Event& e) override {
		_CameraController.OnEvent(e);
	}
private:
	Mochii::ShaderLibrary _ShaderLibrary;
	Mochii::Ref<Mochii::Shader> _Shader;
	Mochii::Ref<Mochii::VertexArray> _VertexArray;

	Mochii::Ref<Mochii::Shader> _FlatColorShader;
	Mochii::Ref<Mochii::VertexArray> _SquareVA;

	Mochii::Ref<Mochii::Texture2D> _Texture;

	Mochii::OrthographicCameraController _CameraController;
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
