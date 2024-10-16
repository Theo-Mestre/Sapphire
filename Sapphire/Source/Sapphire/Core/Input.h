#ifndef SPH_INPUT_H
#define SPH_INPUT_H

#include "Log.h"

namespace sph
{
	class Window;
	
#pragma warning(disable: 5103)
#define INPUT_CALL_IMPL(x) ASSERT(s_instance, "Input not initialized!") return s_instance->##x

	class  Input
	{
	public:
		inline static bool IsKeyPressed(int _key) { INPUT_CALL_IMPL(IsKeyPressedImpl(_key)); }
		inline static bool IsMouseButtonPressed(int _button) { INPUT_CALL_IMPL(IsMouseButtonPressedImpl(_button)); }

		inline static glm::vec2 GetMousePosition() { INPUT_CALL_IMPL(GetMousePositionImpl()); }
		inline static float GetMouseX() { INPUT_CALL_IMPL(GetMouseXImpl()); }
		inline static float GetMouseY() { INPUT_CALL_IMPL(GetMouseYImpl()); }

		inline static void Init(Window* _window)
		{
			ASSERT(s_instance, "Input not initialized!");
			s_instance->m_window = _window;
			LogInfo("Input initialized");
		}

		Input() = default;
		~Input() = default;

	protected:
		virtual bool IsKeyPressedImpl(int _key) = 0;
		virtual bool IsMouseButtonPressedImpl(int _button) = 0;
		virtual glm::vec2 GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	protected:
		Window* m_window = nullptr;
	private:
		static Input* s_instance;
	};
}
#endif