#include "Sapphire.h"

#include "App.h"
#include "DefaultLayer.h"

Sandbox::Sandbox()
	: sph::Application()
{
	Init();
}

void Sandbox::Init()
{
	PushLayer(new DefaultLayer());

	// Init rendered objects

	sph::BufferLayout layout = {
		{ sph::ShaderDataType::Float3, "a_position" },
		{ sph::ShaderDataType::Float4, "a_color" }
	};

	///////////////////////////////////////////
	///			Tri Vertex Array    		///
	///////////////////////////////////////////

	m_triVA.reset(sph::VertexArray::Create());

	float vertices[(3 + 4) * 3] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	// Create vertex buffer
	std::shared_ptr<sph::VertexBuffer> triVB;
	triVB.reset(sph::VertexBuffer::Create(&vertices, sizeof(vertices)));
	triVB->SetLayout(layout);

	// Create index buffer
	uint32_t indices[3] = { 0, 1, 2 };
	std::shared_ptr<sph::IndexBuffer> triIB;
	triIB.reset(sph::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

	// Add buffers to vertex array
	m_triVA->AddVertexBuffer(triVB);
	m_triVA->SetIndexBuffer(triIB);

	
	///////////////////////////////////////////
	///		  Square Vertex Array    		///
	///////////////////////////////////////////

	m_rectVA.reset(sph::VertexArray::Create());

	float rectVertices[(3 + 4) * 4] = {
		-0.75f, -0.75f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		 0.75f, -0.75f, 0.0f, 0.2f, 0.8f, 0.8f, 1.0f,
		 0.75f,  0.75f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		-0.75f,  0.75f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f
	};

	// Create vertex buffer
	std::shared_ptr<sph::VertexBuffer> squareVB;
	squareVB.reset(sph::VertexBuffer::Create(&rectVertices, sizeof(rectVertices)));
	squareVB->SetLayout(layout);

	// Create index buffer
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	std::shared_ptr<sph::IndexBuffer> squareIB;
	squareIB.reset(sph::IndexBuffer::Create(squareIndices, sizeof(rectVertices) / sizeof(uint32_t)));

	// Add buffers to vertex array
	m_rectVA->AddVertexBuffer(squareVB);
	m_rectVA->SetIndexBuffer(squareIB);

	///////////////////////////////////////////
	///				Shader	    			///
	///////////////////////////////////////////

	std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			out vec3 v_position;
			out vec4 v_color;

			void main()
			{
				v_position = a_position;
				v_color = a_color;
				gl_Position = vec4(a_position, 1.0);
			}
		)";

	std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_position;
			in vec4 v_color;

			void main()
			{
				color = v_color;
			}
		)";

	m_shader.reset(new sph::Shader(vertexSrc, fragmentSrc));
}

void Sandbox::OnEvent(sph::Event& _event)
{
	sph::EventDispatcher dispatcher(_event);
	dispatcher.Dispatch<sph::WindowCloseEvent>([&](sph::WindowCloseEvent& _event) { return m_isRunning = false; });

	for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
	{
		(*--it)->OnEvent(_event);
	}

	if (_event.GetEventType() == sph::EventType::KeyPressed)
	{
		sph::KeyPressedEvent& e = (sph::KeyPressedEvent&)_event;
		if (e.GetKeyCode() == sph::KeyCode::A)
		{
			LogInfo("Tab key is pressed (event)!");
		}
	}
}

void Sandbox::OnUpdate()
{
}

void Sandbox::OnRender()
{
	sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	sph::RenderCommand::Clear();

	sph::Renderer::BeginScene();
	{
		// Render square
		sph::Renderer::Submit(m_rectVA, m_shader);

		// Render triangle
		sph::Renderer::Submit(m_triVA, m_shader);
	}
	sph::Renderer::EndScene();
}

sph::Application* sph::CreateApplication()
{
	return new Sandbox();
}