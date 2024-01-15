#include "Sapphire.h"

#include "App.h"
#include "DefaultLayer.h"

#include "Sapphire/Platform/OpenGL/OpenGLShader.h"

#include <glm/glm.hpp>

Sandbox::Sandbox()
	: sph::Application()
{
	Init();
}

void Sandbox::Init()
{
	sph::Renderer::Init();

	m_color1 = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);
	m_color2 = glm::vec4(0.8f, 0.3f, 0.2f, 1.0f);

	m_framebuffer = sph::Framebuffer::Create({ 1280, 720 });

	auto layer = new DefaultLayer();
	layer->SetColors(&m_color1, &m_color2);
	PushLayer(layer);

	// Init rendered objects
	sph::BufferLayout layout = {
		{ sph::ShaderDataType::Float3, "a_position" },
		{ sph::ShaderDataType::Float4, "a_color" }
	};

	sph::BufferLayout layoutSprite = {
		{ sph::ShaderDataType::Float3, "a_position" },
		{ sph::ShaderDataType::Float2, "a_texCoord" }
	};

	m_camera.reset(new sph::OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f));

	///////////////////////////////////////////
	///			Tri Vertex Array    		///
	///////////////////////////////////////////

	m_spriteVA.reset(sph::VertexArray::Create());

	float vertices[(3 + 2) * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	// Create vertex buffer
	std::shared_ptr<sph::VertexBuffer> spriteVB;
	spriteVB.reset(sph::VertexBuffer::Create(&vertices, sizeof(vertices)));
	spriteVB->SetLayout(layoutSprite);

	// Create index buffer
	uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
	std::shared_ptr<sph::IndexBuffer> spriteIB;
	spriteIB.reset(sph::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

	// Add buffers to vertex array
	m_spriteVA->AddVertexBuffer(spriteVB);
	m_spriteVA->SetIndexBuffer(spriteIB);


	///////////////////////////////////////////
	///		  Square Vertex Array    		///
	///////////////////////////////////////////

	m_squareTransform = glm::mat4(1.0f);

	m_rectVA.reset(sph::VertexArray::Create());

	float rectVertices[(3 + 4) * 4] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.8f, 1.0f,
		 0.5f,  0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f
	};

	// Create vertex buffer
	sph::Ref<sph::VertexBuffer> squareVB;
	squareVB.reset(sph::VertexBuffer::Create(&rectVertices, sizeof(rectVertices)));
	squareVB->SetLayout(layout);

	// Create index buffer
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	sph::Ref<sph::IndexBuffer> squareIB;
	squareIB.reset(sph::IndexBuffer::Create(squareIndices, sizeof(rectVertices) / sizeof(uint32_t)));

	// Add buffers to vertex array
	m_rectVA->AddVertexBuffer(squareVB);
	m_rectVA->SetIndexBuffer(squareIB);

	///////////////////////////////////////////
	///				Triangle  				///
	///////////////////////////////////////////

	m_triVA.reset(sph::VertexArray::Create());

	float triVertices[3 * 3] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	sph::Ref<sph::VertexBuffer> triVB;
	triVB.reset(sph::VertexBuffer::Create(&triVertices, sizeof(triVertices)));
	triVB->SetLayout({
		{ sph::ShaderDataType::Float3, "a_position" }
		});

	uint32_t triIndices[3] = { 0,1,2 };
	sph::Ref<sph::IndexBuffer> triIB;
	triIB.reset(sph::IndexBuffer::Create(triIndices, sizeof(triIndices) / sizeof(uint32_t)));

	m_triVA->AddVertexBuffer(triVB);
	m_triVA->SetIndexBuffer(triIB);

	///////////////////////////////////////////
	///				Texture  				///
	///////////////////////////////////////////

	m_texture = sph::Texture2D::Create("SapphireBG.png");
	m_sapphire = sph::Texture2D::Create("Sapphire.png");

	///////////////////////////////////////////
	///				Shader	    			///
	///////////////////////////////////////////

	InitShaders();
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
		if (e.GetKeyCode() == sph::KeyCode::Tab)
		{
			LogInfo("Tab key is pressed (event)!");
		}
	}
}

void Sandbox::OnUpdate()
{
	auto rotation = 20.f * sph::Time::DeltaTime;
	auto translation = 1.f * sph::Time::DeltaTime;

#pragma region Camera Movement
	if (sph::Input::IsKeyPressed(sph::KeyCode::Q))
	{
		m_camera->Rotate(0.0f, 0.0f, rotation);
	}
	if (sph::Input::IsKeyPressed(sph::KeyCode::E))
	{
		m_camera->Rotate(0.0f, 0.0f, -rotation);
	}
	if (sph::Input::IsKeyPressed(sph::KeyCode::D))
	{
		m_camera->Translate(translation, 0.0f, 0.0f);
	}
	if (sph::Input::IsKeyPressed(sph::KeyCode::A))
	{
		m_camera->Translate(-translation, 0.0f, 0.0f);
	}
	if (sph::Input::IsKeyPressed(sph::KeyCode::W))
	{
		m_camera->Translate(0.0f, translation, 0.0f);
	}
	if (sph::Input::IsKeyPressed(sph::KeyCode::S))
	{
		m_camera->Translate(0.0f, -translation, 0.0f);
	}
#pragma endregion

#pragma	region Square transformation
	if (sph::Input::IsKeyPressed(sph::KeyCode::Left))
	{
		m_squareTransform = glm::translate(m_squareTransform, glm::vec3(-translation, 0.0f, 0.0f));
	}
	if (sph::Input::IsKeyPressed(sph::KeyCode::Right))
	{
		m_squareTransform = glm::translate(m_squareTransform, glm::vec3(translation, 0.0f, 0.0f));
	}
	if (sph::Input::IsKeyPressed(sph::KeyCode::Up))
	{
		m_squareTransform = glm::translate(m_squareTransform, glm::vec3(0.0f, translation, 0.0f));
	}
	if (sph::Input::IsKeyPressed(sph::KeyCode::Down))
	{
		m_squareTransform = glm::translate(m_squareTransform, glm::vec3(0.0f, -translation, 0.0f));
	}
	if (sph::Input::IsKeyPressed(sph::KeyCode::R))
	{
		m_squareTransform = glm::rotate(m_squareTransform, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (sph::Input::IsKeyPressed(sph::KeyCode::T))
	{
		m_squareTransform = glm::rotate(m_squareTransform, -rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (sph::Input::IsKeyPressed(sph::KeyCode::Y))
	{
		m_squareTransform = glm::scale(m_squareTransform, glm::vec3(1.0f + translation, 1.0f + translation, 1.0f));
	}
	if (sph::Input::IsKeyPressed(sph::KeyCode::U))
	{
		m_squareTransform = glm::scale(m_squareTransform, glm::vec3(1.0f - translation, 1.0f - translation, 1.0f));
	}

#pragma endregion
}

void Sandbox::OnRender()
{
	m_framebuffer->Bind();

	sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	sph::RenderCommand::Clear();

	sph::Renderer::BeginScene(*m_camera);
	{
		sph::OpenGLShader* shader = static_cast<sph::OpenGLShader*>(m_colorMaterial->GetShader().get());

		// Render squares
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; ++j)
			{
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(i * 0.11f, j * 0.11f, 0.0f));
				transform = glm::scale(transform, glm::vec3(0.1f, 0.1f, 1.0f));

				if (i % 2 + j % 2 == 0)
				{
					shader->SetUniformFloat4("u_color", m_color2);
				}
				else
				{
					shader->SetUniformFloat4("u_color", m_color1);
				}

				sph::Renderer::Submit(m_rectVA, m_colorMaterial, transform);
			}
		}
		static glm::mat4 triTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.80f, -0.40f, 0.0f));

		m_defaultMaterial->Bind();
		static_cast<sph::OpenGLShader*>(m_defaultMaterial->GetShader().get())->SetUniformFloat("u_time", sph::Time::GameTime);
		sph::Renderer::Submit(m_triVA, m_defaultMaterial, triTransform);

		m_textureMaterial->SetTexture(m_texture);
		sph::Renderer::Submit(m_spriteVA, m_textureMaterial, m_squareTransform);


		m_textureMaterial->SetTexture(m_sapphire);
		sph::Renderer::Submit(m_spriteVA, m_textureMaterial, m_squareTransform + glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 0.0f)));
		// Render triangle
		//sph::Renderer::Submit(m_triVA, m_defaultMaterial, m_squareTransform);
	}
	sph::Renderer::EndScene();

	m_framebuffer->Unbind();
}

void Sandbox::OnImGuiRender()
{
	////////////// ImGui Code //////////////


	// Note: Switch this to true to enable dockspace
	static bool dockspaceOpen = true;
	bool opt_fullscreen = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen) ImGui::PopStyleVar(2);

	//////////////  DockSpace //////////////
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit")); // Add exit function here
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Viewport");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

	uint32_t textureID = m_framebuffer->GetColorAttachmentRendererID();
	ImVec2 windowSize = { (float)m_framebuffer->GetSpecification().Width, (float)m_framebuffer->GetSpecification().Height };
	ImGui::Image((void*)textureID, windowSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::Begin("Color");
	ImGui::ColorEdit4("Color 1", glm::value_ptr(m_color1));
	ImGui::ColorEdit4("Color 2", glm::value_ptr(m_color2));
	ImGui::End();

	ImGui::End();
}

void Sandbox::InitShaders()
{
	// Color shader
	std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;

			void main()
			{
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
			}
		)";

	std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			uniform vec4 u_color;

			void main()
			{
				color = u_color;
			}
		)";

	sph::Ref<sph::Shader> shader(sph::Shader::Create(vertexSrc, fragmentSrc));
	m_colorMaterial.reset(new sph::Material(shader));

	// Default shader
	vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;

			out vec3 v_position;

			void main()
			{
				v_position = a_position;
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
			}
		)";

	fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			uniform float u_time;
			in vec3 v_position;

			void main()
			{
				float colorModifier = cos(u_time);
				color = vec4(v_position * 0.5 + (0.5 + colorModifier * 0.5), 1.0);
			}
		)";

	sph::Ref<sph::Shader> defaultShader(sph::Shader::Create(vertexSrc, fragmentSrc));
	m_defaultMaterial.reset(new sph::Material(defaultShader));

	// Color shader
	vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec2 a_texCoord;

			out vec2 v_texCoord;

			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;

			void main()
			{
				v_texCoord = a_texCoord;
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
			}
		)";

	fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec2 v_texCoord;	

			uniform sampler2D u_texture;

			void main()
			{
				vec4 texColor = texture(u_texture, v_texCoord);
				color = texColor;
			}
		)";

	sph::Ref<sph::Shader> textureShader(sph::Shader::Create(vertexSrc, fragmentSrc));
	m_textureMaterial.reset(new sph::Material(textureShader));
}

sph::Application* sph::CreateApplication()
{
	return new Sandbox();
}