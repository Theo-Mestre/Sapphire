#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "Sapphire/Layers/Layer.h"

namespace sph
{
	class Window;

	class SPH_API ImGuiLayer
		: public Layer
	{
	public:
		ImGuiLayer(Window& _window);
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& _event) override;

	private:
		Window& m_window;
		float m_time = 0.0f;
	};
}
#endif
