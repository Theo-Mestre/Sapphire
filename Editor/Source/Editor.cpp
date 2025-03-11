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
			PushLayer(new EditorLayer());
		}
	};

	sph::Application* sph::CreateApplication()
	{
		return new EditorApp();
	}
}