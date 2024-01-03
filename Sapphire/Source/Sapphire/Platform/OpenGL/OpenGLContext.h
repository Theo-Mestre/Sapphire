#ifndef SPH_OPENGL_CONTEXT_H
#define SPH_OPENGL_CONTEXT_H

#include "Sapphire/Renderer/GraphicsContext.h"

namespace sph
{
	class OpenGLContext
		: public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* _window);
		virtual ~OpenGLContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
	};
}
#endif