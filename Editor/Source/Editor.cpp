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
			SPH_PROFILE_FUNCTION();

			SetRenderer(sph::BatchRenderer2D::Create());

			Application::Init();

			glm::vec2 halfWindowSize = m_window->GetSize() / 2.0f;
			m_camera = sph::CreateRef<sph::OrthographicCamera>(-halfWindowSize.x, halfWindowSize.x, -halfWindowSize.y, halfWindowSize.y);

			auto editorLayer = new EditorLayer();
			editorLayer->SetCamera(m_camera);
			editorLayer->SetImGUILayer(m_imGuiLayer);
			PushLayer(editorLayer);
		}
	private:
		sph::Ref<sph::OrthographicCamera> m_camera = nullptr;
	};

	sph::Application* sph::CreateApplication()
	{
		return new EditorApp();
	}
}