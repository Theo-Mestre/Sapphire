#include "Sapphire.h"
#include "Sapphire/Core/EntryPoint.h"

#include "Layers/EditorLayer.h"

namespace sph
{
	class EditorApp
		: public sph::Application
	{
	public:
		virtual void Init() override
		{
			SetRenderer(sph::BatchRenderer2D::Create());
			Application::Init();

			glm::vec2 halfWindowSize = m_window->GetSize() / 2.0f;
			m_camera = sph::CreateRef<sph::OrthographicCamera>(-halfWindowSize.x, halfWindowSize.x, -halfWindowSize.y, halfWindowSize.y);

			auto testLayer = new EditorLayer();
			testLayer->SetCamera(m_camera);
			testLayer->SetImGUILayer(m_imGuiLayer);
			PushLayer(testLayer);
		}

	private:
		sph::Ref<sph::OrthographicCamera> m_camera = nullptr;
	};

	sph::Application* sph::CreateApplication()
	{
		return new EditorApp();
	}
}