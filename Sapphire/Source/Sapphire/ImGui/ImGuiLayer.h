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

	class  ImGuiLayer
		: public Layer
	{
	public:
		ImGuiLayer(Window& _window);
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(DeltaTime _dt) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void EnableDocking(bool _enable) { m_enableDocking = _enable; }
		bool IsDockingEnabled() const { return m_enableDocking; }
	private:
		Window& m_window;
		float m_time = 0.0f;
		bool m_enableDocking = true;
	};
}
#endif
