#include "sphpch.h"

#include "SceneSerializer.h"

#include "Sapphire/Core/Log.h"
#include "Sapphire/Scene/Scene.h"
#include "Sapphire/Scene/Components.h"
#include "Sapphire/Scene/Entity.h"

namespace sph
{
	static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq;
		out << v.x << v.y << v.z;
		out << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& _scene)
		: m_scene(_scene)
	{

	}
	void SceneSerializer::Serialize(const std::string& _filepath)
	{
		if (m_scene == nullptr)
		{
			LogError("SceneSerializer: Scene is null");
			return;
		}
		if (_filepath.empty())
		{
			LogError("SceneSerializer: Filepath is empty");
			return;
		}

		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene";
		out << YAML::Value << "Untitled";
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;

		for (auto view : m_scene->m_registry.view<TransformComponent, TagComponent>())
		{
			Entity entity = { view, m_scene.get()};
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::BeginMap;
			out << YAML::Key << "Entity" << YAML::Value << tag;
			if (entity.HasComponent<TransformComponent>())
			{
				out << YAML::Key << "Components" << YAML::Value << YAML::BeginMap;
				out << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginMap;
				auto& transform = entity.GetComponent<TransformComponent>();
				out << YAML::Key << "Translation" << YAML::Value << transform.Translation;
				out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
				out << YAML::Key << "Scale" << YAML::Value << transform.Scale;
				out << YAML::EndMap;
				out << YAML::EndMap;
			}
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
		out << YAML::EndSeq;

		std::ofstream file(_filepath, std::ios::out);
		file << out.c_str();

		LogInfo("SceneSerializer: Scene saved to {0}", _filepath);

		file.close();
	}

	bool SceneSerializer::Deserialize(const std::string& _filepath)
	{
		return false;
	}
}