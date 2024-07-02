#include <glm/glm.hpp>

#include "Sapphire.h"
#include "Sapphire/EntryPoint.h"

#include "App.h"
#include "DefaultLayer.h"
#include "Layers/TestRenderer.h"

#include "Sapphire/Platform/OpenGL/OpenGLShader.h"


Sandbox::Sandbox()
	: sph::Application()
{
	Init();
}

void Sandbox::Init()
{
	m_color1 = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);
	m_color2 = glm::vec4(0.8f, 0.3f, 0.2f, 1.0f);

	m_framebuffer = sph::Framebuffer::Create({ 1280, 720 });

	//auto layer = new DefaultLayer();
	//layer->SetColors(&m_color1, &m_color2);
	//PushLayer(layer);

	PushLayer(new TestRenderer());

	// Init rendered objects
	sph::BufferLayout layout = {
		{ sph::ShaderDataType::Float3, "a_position" },
		{ sph::ShaderDataType::Float4, "a_color" }
	};

	sph::BufferLayout layoutSprite = {
		{ sph::ShaderDataType::Float3, "a_position" },
		{ sph::ShaderDataType::Float2, "a_texCoord" }
	};

	m_cameraController = sph::CreateScope<sph::OrthographicCameraController>(m_window->GetWidth() / (float)m_window->GetHeight(), true);

	///////////////////////////////////////////
	///			Tri Vertex Array    		///
	///////////////////////////////////////////

	m_spriteVA = sph::VertexArray::Create();

	float vertices[(3 + 2) * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	// Create vertex buffer
	std::shared_ptr<sph::VertexBuffer> spriteVB;
	spriteVB = sph::VertexBuffer::Create(&vertices, sizeof(vertices));
	spriteVB->SetLayout(layoutSprite);

	// Create index buffer
	uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
	std::shared_ptr<sph::IndexBuffer> spriteIB;
	spriteIB = sph::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

	// Add buffers to vertex array
	m_spriteVA->AddVertexBuffer(spriteVB);
	m_spriteVA->SetIndexBuffer(spriteIB);


	///////////////////////////////////////////
	///		  Square Vertex Array    		///
	///////////////////////////////////////////

	m_squareTransform = glm::mat4(1.0f);

	m_rectVA = sph::VertexArray::Create();

	float rectVertices[(3 + 4) * 4] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.8f, 1.0f,
		 0.5f,  0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f
	};

	// Create vertex buffer
	sph::Ref<sph::VertexBuffer> squareVB;
	squareVB = sph::VertexBuffer::Create(&rectVertices, sizeof(rectVertices));
	squareVB->SetLayout(layout);

	// Create index buffer
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	sph::Ref<sph::IndexBuffer> squareIB;
	squareIB = sph::IndexBuffer::Create(squareIndices, sizeof(rectVertices) / sizeof(uint32_t));

	// Add buffers to vertex array
	m_rectVA->AddVertexBuffer(squareVB);
	m_rectVA->SetIndexBuffer(squareIB);

	///////////////////////////////////////////
	///				Triangle  				///
	///////////////////////////////////////////

	m_triVA = sph::VertexArray::Create();

	float triVertices[3 * 3] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	sph::Ref<sph::VertexBuffer> triVB;
	triVB = sph::VertexBuffer::Create(&triVertices, sizeof(triVertices));
	triVB->SetLayout({
		{ sph::ShaderDataType::Float3, "a_position" }
		});

	uint32_t triIndices[3] = { 0,1,2 };
	sph::Ref<sph::IndexBuffer> triIB;
	triIB = sph::IndexBuffer::Create(triIndices, sizeof(triIndices) / sizeof(uint32_t));

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
	m_cameraController->OnEvent(_event);

	for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
	{
		(*--it)->OnEvent(_event);
	}
}

void Sandbox::OnUpdate()
{
	m_cameraController->OnUpdate(sph::Time::DeltaTime);
}

void Sandbox::OnRender()
{
	m_framebuffer->Bind();

	sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	sph::RenderCommand::Clear();

	sph::Renderer::BeginScene(m_cameraController->GetCamera());
	{
		sph::OpenGLShader* colorShader = static_cast<sph::OpenGLShader*>(m_colorMaterial->GetShader().get());

		// Render squares
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; ++j)
			{
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(i * 0.11f, j * 0.11f, 0.0f));
				transform = glm::scale(transform, glm::vec3(0.1f, 0.1f, 1.0f));

				if (i % 2 + j % 2 == 0)
				{
					colorShader->UploadUniformFloat4("u_color", m_color2);
				}
				else
				{
					colorShader->UploadUniformFloat4("u_color", m_color1);
				}

				sph::Renderer::Submit(m_rectVA, m_colorMaterial, transform);
			}
		}
		static glm::mat4 triTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.80f, -0.40f, 0.0f));

		m_defaultMaterial->Bind();
		static_cast<sph::OpenGLShader*>(m_defaultMaterial->GetShader().get())->UploadUniformFloat("u_time", sph::Time::GameTime);
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

	ImGui::End();
}

void Sandbox::InitShaders()
{
	m_shaderLibrary.LoadShader("Shaders/Color.glsl");
	m_shaderLibrary.LoadShader("Shaders/ColorFlow.glsl");
	m_shaderLibrary.LoadShader("Shaders/Texture.glsl");

	m_colorMaterial.reset(new sph::Material(m_shaderLibrary.GetShader("Color")));
	m_defaultMaterial.reset(new sph::Material(m_shaderLibrary.GetShader("ColorFlow")));
	m_textureMaterial.reset(new sph::Material(m_shaderLibrary.GetShader("Texture")));
}



class Sandbox2D
	: public sph::Application
{
public:
	Sandbox2D()
	{
		PushLayer(new TestRenderer());
	}
	virtual ~Sandbox2D() = default;
};

sph::Application* sph::CreateApplication()
{
	return new Sandbox2D();
}