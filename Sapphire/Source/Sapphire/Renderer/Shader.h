#ifndef SPH_SHADER_H
#define SPH_SHADER_H

#include <string>

#include "Sapphire/Core/Core.h"

namespace sph
{
	class Shader
	{
	public:
		Shader() = default;
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<Shader> Create(const std::string& _filePath);

		static Ref<Shader> Create(const std::string& _name, const std::string& _vertexSource, const std::string& _fragmentSource);

		const std::string& GetName() const { return m_name; }
		void SetName(const std::string& _name) { m_name = _name; }

		virtual void SetInt(const std::string& _name, int _value) = 0;
		virtual void SetIntArray(const std::string& _name, int* _values, uint32_t _count) = 0;
		virtual void SetFloat(const std::string& _name, float _value) = 0;
		virtual void SetFloat2(const std::string& _name, const glm::vec2& _value) = 0;
		virtual void SetFloat3(const std::string& _name, const glm::vec3& _value) = 0;
		virtual void SetFloat4(const std::string& _name, const glm::vec4& _value) = 0;
		virtual void SetMat3(const std::string& _name, const glm::mat3& _matrix) = 0;
		virtual void SetMat4(const std::string& _name, const glm::mat4& _matrix) = 0;

	protected:
		std::string m_name;
	};

	class ShaderLibrary
	{
	public:
		void AddShader(const sph::Ref<Shader>& _shader);
		void LoadShader(const std::string& _filePath);

		sph::Ref<Shader> GetShader(const std::string& _name);
		void RemoveShader(const std::string& _name);
		void RemoveShader(const sph::Ref<Shader>& _shader);

		bool Exists(const std::string& _name) const;
	private:
		std::unordered_map<std::string, sph::Ref<Shader>> m_shaders;
	};
}
#endif