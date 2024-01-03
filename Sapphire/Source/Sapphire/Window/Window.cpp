#include "sphpch.h"
#include "Window.h"

namespace sph
{
		WindowProperties::WindowProperties(uint32_t _width, uint32_t _height, const std::string& _title, EventCallbackFunction _callback, GLFWmonitor* _monitor, GLFWwindow* _share)
		: Width(_width)
		, Height(_height)
		, Title(_title)
		, Monitor(_monitor)
		, Share(_share)
	{	}
}