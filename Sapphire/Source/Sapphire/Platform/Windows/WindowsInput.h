#ifndef SPH_WINDOWS_INPUT_H
#define SPH_WINDOWS_INPUT_H

#include "Sapphire/Core/Input.h"

namespace sph
{

	class WindowsInput 
		: public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;

		virtual glm::vec2 GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}

#endif