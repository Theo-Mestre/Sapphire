#ifndef SPH_LIGHT_H
#define SPH_LIGHT_H

#include "Sapphire.h"

struct Light
{
	glm::vec4 color;
	glm::vec2 position;
	float intensity;
	float radius;

	inline static sph::BufferElement CreateLightBufferElement()
	{
		sph::BufferElement element;
		element.type = sph::ShaderDataType::Custom;
		element.name = "LightsData";
		element.size = sizeof(Light);
		element.offset = 0;
		element.normalized = false;
		return element;
	}
};

class LightData
{
public:
	LightData()
		: m_lightShader(nullptr)
		, m_uniformBuffer(nullptr)
		, m_layout()
		, m_lightCount(0)
	{
		m_lightShader = sph::Shader::Create("Shaders/LightShader.glsl");
	}

	~LightData()
	{

	}

	void UploadLightsData(const std::vector<Light>& _lights)
	{
		m_lightCount = (uint32_t)_lights.size();
		if (m_lightCount <= 0) return;

		m_layout.AddElement(sph::ShaderDataType::Float4, "LightsCount");
		for (uint32_t i = 0; i < m_lightCount; i++)
		{
			m_layout.AddCustomElement(Light::CreateLightBufferElement());
		}

		char* blockBuffer = new char[m_layout.GetStride()];
		auto elements = m_layout.GetElements();

		memcpy(blockBuffer + elements[0].offset, &m_lightCount, sizeof(uint32_t));
		uint32_t lightID = 0;
		for (uint32_t i = 1; i <= m_lightCount; i++)
		{
			memcpy(blockBuffer + elements[i].offset, &_lights[i - 1], sizeof(Light));
		}

		// Create Uniform Buffer Object
		m_uniformBuffer = sph::UniformBuffer::Create(m_layout, 1);
		m_uniformBuffer->SetData(blockBuffer, m_layout.GetStride());

		delete[] blockBuffer;
	}

	const sph::Ref<sph::Shader>& GetShader() const { return m_lightShader; }
	const sph::Ref<sph::UniformBuffer>& GetUniformBuffer() const { return m_uniformBuffer; }

private:
	sph::Ref<sph::Shader> m_lightShader;
	sph::Ref<sph::UniformBuffer> m_uniformBuffer;
	sph::UniformBufferLayout m_layout;
	uint32_t m_lightCount;

};
#endif