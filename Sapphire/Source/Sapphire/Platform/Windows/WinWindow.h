#ifndef CORE_WINDOW_H
#define CORE_WINDOW_H

#include "Sapphire/Core.h"
#include "Sapphire/Window/Window.h"

namespace sph
{
	/**
	 * @brief Windows implementation of the Window class
	 */
	class SPH_API WinWindow
		: public Window
	{
	public:
		WinWindow(const WindowProperties& _properties);
		~WinWindow();

		void OnUpdate(void) override;

		inline int GetWidth(void) const override { return m_data.Width; }
		inline int GetHeight(void) const override { return m_data.Height; }

		inline void SetEventCallback(const EventCallbackFunction& _callback) override { m_data.EventCallback = _callback; }

	private:
		virtual void Init(const WindowProperties& _properties);
		virtual void InitEventCallbacks(void);
		virtual void Shutdown(void);

		struct WindowData
		{
			std::string Title = "";
			uint32_t Width = 0;
			uint32_t Height = 0;

			EventCallbackFunction EventCallback = nullptr;
		};

	private:
		inline static bool s_GLFWInitialized = false;
		GLFWwindow* m_window;
		WindowData m_data;
	};
}
#endif