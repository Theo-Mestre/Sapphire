#ifndef SPH_EVENT_H
#define SPH_EVENT_H

#include "sphpch.h"
#include "Sapphire/Core/Core.h"

namespace sph
{
	enum class EventType
	{
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		AppTick, AppUpdate, AppRender,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};


#pragma warning(disable: 5103)
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	// Base event class for all events types
	class  Event
	{
	public:
		virtual ~Event() = default;
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) const
		{
			return GetCategoryFlags() & category;
		}

		friend std::ostream& operator<<(std::ostream& _os, const Event& _e)
		{
			return _os << _e.ToString();
		}

		bool Handled = false;
		friend class EventDispatcher;
	};
	using EventCallbackFunction = std::function<void(Event&)>;

	class  EventDispatcher
	{
	public:
		EventDispatcher(Event& _event)
			: m_event(_event) {		}

		template<typename T, typename F>
		bool Dispatch(const F& _func)
		{
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.Handled |= _func(static_cast<T&>(m_event));
				return true;
			}
			return false;
		}
	private:
		Event& m_event;
	};
}
#endif