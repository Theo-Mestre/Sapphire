#include "sphpch.h"
#include "Renderer.h"

#include "Sapphire/Renderer//RenderCommand.h"

namespace sph
{
	void Renderer::OnWindowResize(uint32_t _width, uint32_t _height)
	{
		RenderCommand::SetViewport(0, 0, _width, _height);
		m_screenSize = { _width, _height };
	}

	glm::vec3&& Renderer::NormalizePosition(const glm::vec3& _position)
	{
		return (_position / glm::vec3(m_screenSize, 1.0f)) * 2.0f - 1.0f;
	}
	glm::vec2&& Renderer::NormalizeSize(const glm::vec2& _size)
	{
		return _size / m_screenSize;
	}
}