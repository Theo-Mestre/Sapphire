#ifndef SAPPHIRE_LAYER_H
#define SAPPHIRE_LAYER_H

#include "Sapphire/Events/Event.h"
#include "Sapphire/Time.h"

namespace sph
{
	class  Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(DeltaTime _dt) {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		inline const std::string& GetName() const { return m_name; }
	protected:
		std::string m_name;
	};
}
#endif