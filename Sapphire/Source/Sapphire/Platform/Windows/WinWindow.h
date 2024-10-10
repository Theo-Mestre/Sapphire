#ifndef SPH_WINDOW_H
#define SPH_WINDOW_H

#include "Sapphire/Core/Core.h"
#include "Sapphire/Window/Window.h"

namespace sph
{
	class  GraphicsContext;

	/**
	 * @brief Windows implementation of the Window class
	 */
	class  WinWindow
		: public Window
	{
	public:
		WinWindow(const WindowProperties& _properties);
		~WinWindow();

		void virtual OnUpdate(void) override;

		inline virtual int GetWidth(void) const override { return m_data.Width; }
		inline virtual int GetHeight(void) const override { return m_data.Height; }

		inline virtual void SetEventCallback(const EventCallbackFunction& _callback) override { m_data.EventCallback = _callback; }

		inline virtual void* GetNativeWindow(void) const override { return m_window; }

		virtual void SetVSync(bool enabled) override;

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
		GraphicsContext* m_context;
	};
}
#endif