#ifndef SPH_APPLICATION_EVENT_H
#define SPH_APPLICATION_EVENT_H

#include "sphpch.h"
#include "Event.h"

namespace sph
{
	class  ApplicationEvent : public Event
	{
	public:
		ApplicationEvent() = default;
		~ApplicationEvent() = default;

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class  WindowResizeEvent : public ApplicationEvent
	{

	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)

	private:
		unsigned int m_Width, m_Height;
	};

	class  WindowCloseEvent : public ApplicationEvent
	{
	public:
		WindowCloseEvent() = default;
		~WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
	};

	class  AppTickEvent : public ApplicationEvent
	{
	public:
		AppTickEvent() = default;
		~AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
	};

	class  AppUpdateEvent : public ApplicationEvent
	{
	public:
		AppUpdateEvent() = default;
		~AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
	};

	class  AppRenderEvent : public ApplicationEvent
	{
	public:
		AppRenderEvent() = default;
		~AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
	};
}

#endif