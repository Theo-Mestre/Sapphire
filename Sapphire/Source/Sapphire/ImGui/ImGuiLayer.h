#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "Sapphire/Layers/Layer.h"

namespace sph
{
	class Window;
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;
	class MouseMovedEvent;
	class MouseScrolledEvent;
	class KeyPressedEvent;
	class KeyReleasedEvent;
	class WindowResizeEvent;
	class KeyTypedEvent;

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
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& _event);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& _event);
		bool OnMouseMovedEvent(MouseMovedEvent& _event);
		bool OnMouseScrolledEvent(MouseScrolledEvent& _event);
		bool OnKeyPressedEvent(KeyPressedEvent& _event);
		bool OnKeyReleasedEvent(KeyReleasedEvent& _event);
		bool OnKeyTypedEvent(KeyTypedEvent& _event);
		bool OnWindowResizeEvent(WindowResizeEvent& _event);

	private:
		Window& m_window;
		float m_time = 0.0f;
	};
}
#endif
