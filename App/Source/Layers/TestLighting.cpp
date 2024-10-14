#include "glad/glad.h"

#include "TestLighting.h"

#define USE_UNIFORM_BUFFER_OBJECT 1

TestLighting::TestLighting(sph::Application* const _app)
	: Layer("TestLighting")
	, m_app(_app)
{
}

TestLighting::~TestLighting()
{
}

void TestLighting::OnAttach()
{
	m_cameraController = sph::CreateScope<sph::OrthographicCameraController>(1280.0f / 720.0f, true);

	m_renderer2D = sph::CreateScope<sph::Renderer2D>();
	m_renderer2D->Init();

	m_lightShader = sph::Shader::Create("Shaders/LightShader.glsl");
	m_texture = sph::Texture2D::Create("Background.png");

	sph::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });

	m_ambiantLightColor = { 0.0f, 0.0f, 0.0f };

	// Create Lights
	constexpr uint32_t lightCount = 4;
	Light lights[lightCount] =
	{
		{ { 1.0f, 1.0f, 1.0f , 1.0f}, { .5f, .45f }, 1.f, 0.2f },
		{ { 1.0f, 1.0f, 1.0f , 1.0f}, { .5f, .65f }, 1.f, 0.2f },
		{ { 1.0f, 1.0f, 1.0f , 1.0f}, { .35f, .5f }, 1.f, 0.2f },
		{ { 1.0f, 1.0f, 1.0f , 1.0f}, { .65f, .5f }, 1.f, 0.2f }
	};


	// Create Uniform Buffer Layout
	sph::UniformBufferLayout layout = { { sph::ShaderDataType::Float4, "numLights" }, };
	for (uint32_t i = 0; i < lightCount; i++)
	{
		layout.AddElement(sph::ShaderDataType::Float4, "lights[" + std::to_string(i) + "].color");
		layout.AddElement(sph::ShaderDataType::Float2, "lights[" + std::to_string(i) + "].position");
		layout.AddElement(sph::ShaderDataType::Float, "lights[" + std::to_string(i) + "].intensity");
		layout.AddElement(sph::ShaderDataType::Float, "lights[" + std::to_string(i) + "].radius");
	}

	// Create Buffer Data
	char* blockBuffer = new char[layout.GetStride()];
	auto elements = layout.GetElements();

	memcpy(blockBuffer + elements[0].offset, &lightCount, sizeof(int));
	uint32_t lightID = 0;
	for (uint32_t i = 0; i < lightCount * 4; i += 4)
	{
		memcpy(blockBuffer + elements[i + 1].offset, &lights[lightID], sizeof(Light));

		lightID++;
	}

	// Create Uniform Buffer Object
	m_ubo = sph::UniformBuffer::Create(layout, 0);
	m_ubo->SetData(blockBuffer, layout.GetStride());

	delete[] blockBuffer;
}

void TestLighting::OnDetach()
{
	glDeleteBuffers(1, &m_UBOhandmade);

	m_renderer2D->Shutdown();
}

void TestLighting::OnUpdate(sph::DeltaTime _dt)
{
	m_cameraController->OnUpdate(_dt);

	glm::vec2 mousePosition = { sph::Input::GetMouseX() / 1280, 1 - sph::Input::GetMouseY() / 720 };

	m_ubo->SetData(&mousePosition, sizeof(glm::vec2), 128);
}

void TestLighting::OnRender(const sph::Ref<sph::Renderer>& _renderer)
{
	sph::Renderer2D::s_stats.Reset();
	sph::RenderCommand::Clear();

	m_renderer2D->BeginScene(m_cameraController->GetCamera());

	m_lightShader->Bind();
	m_lightShader->SetMat4("u_viewProjection", m_cameraController->GetCamera().GetViewProjectionMatrix());
	m_lightShader->SetFloat3("u_ambientLight", m_ambiantLightColor);

	m_ubo->Bind();

	m_renderer2D->DrawQuad({ 0.0f, 0.0f, 0.0f }, glm::vec2(2.0f, 1.0f), m_texture, m_lightShader);

	m_renderer2D->EndScene();
}

void TestLighting::OnImGuiRender()
{
}

void TestLighting::OnEvent(sph::Event& _event)
{
	m_cameraController->OnEvent(_event);
}