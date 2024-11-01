#ifndef SAPPHIRE_LAYER_H
#define SAPPHIRE_LAYER_H

#include "Sapphire/Events/Event.h"
#include "Sapphire/Core/Time.h"

namespace sph
{
	class Renderer;
	class Application;

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(DeltaTime _dt) {}
		virtual void OnRender(const Ref<Renderer>& _renderer) {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		inline void SetApplication(Application* _application) { m_application = _application; }
		inline Application* const GetApplication() const { return m_application; }

		inline const std::string& GetName() const { return m_name; }
	protected:
		std::string m_name;
		Application* m_application;
	};
}
#endif