#include "Sapphire.h"
#include "Sapphire/Core/EntryPoint.h"

#include "Layers/TestLayer.h"
#include "Layers/TilemapLayer.h"


class Sandbox2D
	: public sph::Application
{
public:
	virtual void Init() override
	{
		Application::Init();

		glm::vec2 halfWindowSize = m_window->GetSize() / 2.0f;
		m_camera = sph::CreateRef<sph::OrthographicCamera>(-halfWindowSize.x, halfWindowSize.x, -halfWindowSize.y, halfWindowSize.y);

		//auto tilemapLayer = new TilemapLayer();
		//tilemapLayer->SetCamera(m_camera);

		auto testLayer = new TestLayer();
		testLayer->SetCamera(m_camera);
		
		//PushLayer(tilemapLayer);
		PushLayer(testLayer);
	}

private:
	sph::Ref<sph::OrthographicCamera> m_camera = nullptr;
};

sph::Application* sph::CreateApplication()
{
	return new Sandbox2D();
}