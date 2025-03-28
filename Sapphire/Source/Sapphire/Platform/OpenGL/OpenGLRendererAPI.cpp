#include "sphpch.h"
#include <glad/glad.h>

#include "OpenGLRendererAPI.h"
#include "Sapphire/Renderer/Buffer.h"
#include "Sapphire/Renderer/VertexArray.h"
#include "Sapphire/Core/Log.h"

static void OpenGLMessageCallback(unsigned _source, unsigned _type, unsigned _id, unsigned _severity, int _length, const char* _message, const void* _userParam)
{
	SPH_PROFILE_FUNCTION();

	switch (_severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:			LogCritical(_message); return;
	case GL_DEBUG_SEVERITY_MEDIUM:			LogError(_message); return;
	case GL_DEBUG_SEVERITY_LOW:				LogWarn(_message); return;
	case GL_DEBUG_SEVERITY_NOTIFICATION:	LogTrace(_message); return;
	}

	ASSERT(false, "Unknown severity level!");
}

void sph::OpenGLRendererAPI::Init()
{
	SPH_PROFILE_FUNCTION();

#ifdef DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLMessageCallback, nullptr);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
}

void sph::OpenGLRendererAPI::SetViewport(uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height)
{
	SPH_PROFILE_FUNCTION();

	glViewport(_x, _y, _width, _height);
}

void sph::OpenGLRendererAPI::SetClearColor(const glm::vec4& _color)
{
	SPH_PROFILE_FUNCTION();

	glClearColor(_color.r, _color.g, _color.b, _color.a);
}

void sph::OpenGLRendererAPI::Clear()
{
	SPH_PROFILE_FUNCTION();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void sph::OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& _vertexArray, uint32_t _indexCount)
{
	SPH_PROFILE_FUNCTION();

	uint32_t count = _indexCount ? _indexCount : _vertexArray->GetIndexBuffer()->GetCount();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}
