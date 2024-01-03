#ifndef SPH_GRAPHICS_CONTEXT_H
#define SPH_GRAPHICS_CONTEXT_H

struct GLFWwindow;

namespace sph
{
	class GraphicsContext
	{
	public:
		GraphicsContext(GLFWwindow* _window)
			: m_windowHandle(_window) {}
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

	protected:
		GLFWwindow* m_windowHandle;
	};
}
#endif