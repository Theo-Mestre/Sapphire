#include "sphpch.h"
#include <glad/glad.h>

#include "OpenGLRendererAPI.h"
#include "Sapphire/Renderer/Buffer.h"
#include "Sapphire/Renderer/VertexArray.h"

void sph::OpenGLRendererAPI::Init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void sph::OpenGLRendererAPI::SetViewport(uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height)
{
	glViewport(_x, _y, _width, _height);
}

void sph::OpenGLRendererAPI::SetClearColor(const glm::vec4& _color)
{
	glClearColor(_color.r, _color.g, _color.b, _color.a);
}

void sph::OpenGLRendererAPI::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void sph::OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& _vertexArray, uint32_t _indexCount)
{
	uint32_t count = _indexCount ? _indexCount : _vertexArray->GetIndexBuffer()->GetCount();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}
