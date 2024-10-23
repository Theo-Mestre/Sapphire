#ifndef SPH_WINDOW_INTERFACE_H
#define SPH_WINDOW_INTERFACE_H

#include "Sapphire/Core/Core.h"
#include "Sapphire/Events/Event.h"

struct GLFWwindow;
struct GLFWmonitor;

namespace sph
{
	/**
	 * @brief Window properties
	 */
	struct  WindowProperties
	{
		uint32_t Width;
		uint32_t Height;
		std::string Title;
		GLFWmonitor* Monitor;
		GLFWwindow* Share;
		EventCallbackFunction EventCallback;

		WindowProperties(
			uint32_t width = 1280,
			uint32_t height = 720,
			const std::string& title = "Application",
			EventCallbackFunction callback = nullptr,
			GLFWmonitor* _monitor = nullptr,
			GLFWwindow* _share = nullptr);
	};

	/**
	* @brief Window interface
	*/
	class  Window
	{
	public:
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual glm::vec2 GetSize() const = 0;

		virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProperties& properties = WindowProperties());

		virtual void SetVSync(bool enabled) = 0;
	};
}
#endif