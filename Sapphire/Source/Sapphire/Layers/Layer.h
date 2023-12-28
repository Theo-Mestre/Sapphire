#ifndef SAPPHIRE_LAYER_H
#define SAPPHIRE_LAYER_H

#include "Sapphire/Events/Event.h"

namespace sph
{
	class SPH_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_name; }
	protected:
		std::string m_name;
	};
}
#endif