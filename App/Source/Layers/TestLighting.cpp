#include "glad/glad.h"

#include "TestLighting.h"

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

	m_ambiantLightColor = { 0.1f, 0.1f, 0.f };

	GLint structIndex = glGetUniformBlockIndex(m_lightShader->GetRendererID(), "LightBlock");
	LogDebug("LightData index: {0}", structIndex);

	
	glGetActiveUniformBlockiv(m_lightShader->GetRendererID(), structIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

	LogDebug("LightData size: {0}", blockSize);

	GLubyte* blockBuffer = new GLubyte[blockSize];

	const GLchar* names[] = { "numLights", "lights.position", "lights.color", "lights.intensity", "lights.radius" };

	GLuint indices[5];
	glGetUniformIndices(m_lightShader->GetRendererID(), 5, names, indices);

	for (int i = 0; i < 5; i++)
	{
		LogDebug("LightData index {0}: {1}", names[i], indices[i]);
	}

	GLint offset[5];
	glGetActiveUniformsiv(m_lightShader->GetRendererID(), 5, indices, GL_UNIFORM_OFFSET, offset);

	for (int i = 0; i < 5; i++)
	{
		LogDebug("LightData offset {0}: {1}", names[i], offset[i]);
	}

	Light lightData = { { .5f, .5f }, { 0.8f, 0.5f, 0.7f }, 5.0f, 0.5f };
	int lightCount = 1;

	memcpy(blockBuffer + offset[0], &lightCount, sizeof(int));
	memcpy(blockBuffer + offset[1], &lightData.position, sizeof(glm::vec2));
	memcpy(blockBuffer + offset[2], &lightData.color, sizeof(glm::vec3));
	memcpy(blockBuffer + offset[3], &lightData.intensity, sizeof(float));
	memcpy(blockBuffer + offset[4], &lightData.radius, sizeof(float));

	glCreateBuffers(1, &m_UBO);
	glNamedBufferStorage(m_UBO, blockSize, blockBuffer, GL_DYNAMIC_STORAGE_BIT);

	delete[] blockBuffer;
}

void TestLighting::OnDetach()
{
	glDeleteBuffers(1, &m_UBO);

	m_renderer2D->Shutdown();
}

void TestLighting::OnUpdate(sph::DeltaTime _dt)
{
	m_cameraController->OnUpdate(_dt);
}

void TestLighting::OnRender(const sph::Ref<sph::Renderer>& _renderer)
{
	sph::Renderer2D::s_stats.Reset();
	sph::RenderCommand::Clear();

	m_renderer2D->BeginScene(m_cameraController->GetCamera());

	m_lightShader->Bind();
	m_lightShader->SetMat4("u_viewProjection", m_cameraController->GetCamera().GetViewProjectionMatrix());
	m_lightShader->SetFloat3("u_ambientLight", m_ambiantLightColor);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBO, 0, blockSize);

	//m_lightShader->SetInt("u_lightCount", m_lightData.count);

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