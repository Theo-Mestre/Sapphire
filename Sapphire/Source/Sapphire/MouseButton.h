#ifndef SPH_MOUSE_BUTTON_H
#define SPH_MOUSE_BUTTON_H

namespace sph
{
	namespace mouse
	{
		enum class Button
		{
			// From glfw3.h
			Button1 = 0,
			Button2 = 1,
			Button3 = 2,
			Button4 = 3,
			Button5 = 4,
			Button6 = 5,
			Button7 = 6,
			Button8 = 7,
			Last = Button8,

			Left = Button1,
			Right = Button2,
			Middle = Button3
		};
	}

}

#endif