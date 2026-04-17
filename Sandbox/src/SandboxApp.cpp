#include <Mochii.h>
#include "imgui.h"

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

		std::shared_ptr<Mochii::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Mochii::VertexBuffer::Create(vertices, sizeof(vertices)));
		Mochii::BufferLayout layout = {
			{ Mochii::ShaderDataType::Float3, "a_Position" },
			{ Mochii::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Mochii::IndexBuffer> indexBuffer;
		indexBuffer.reset(Mochii::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		_VertexArray->SetIndexBuffer(indexBuffer);

		_SquareVA.reset(Mochii::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Mochii::VertexBuffer> squareVB;
		squareVB.reset(Mochii::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Mochii::ShaderDataType::Float3, "a_Position" }
			});
		_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Mochii::IndexBuffer> squareIB;
		squareIB.reset(Mochii::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		_Shader.reset(new Mochii::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		_BlueShader.reset(new Mochii::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
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

		Mochii::Renderer::Submit(_BlueShader, _SquareVA);
		Mochii::Renderer::Submit(_Shader, _VertexArray);

		Mochii::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
	
	}

	void OnEvent(Mochii::Event& event) override {
	
	}
private:
	std::shared_ptr<Mochii::Shader> _Shader;
	std::shared_ptr<Mochii::VertexArray> _VertexArray;

	std::shared_ptr<Mochii::Shader> _BlueShader;
	std::shared_ptr<Mochii::VertexArray> _SquareVA;

	Mochii::OrthographicCamera _Camera;
	glm::vec3 _CameraPosition;
	float _CameraMoveSpeed = 5.0f;

	float _CameraRotation = 0.0f;
	float _CameraRotationSpeed = 180.0f;
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
