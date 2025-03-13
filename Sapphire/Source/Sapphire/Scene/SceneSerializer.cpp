#include "sphpch.h"

#include "SceneSerializer.h"

#include "Sapphire/Core/Log.h"
#include "Sapphire/Scene/Scene.h"
#include "Sapphire/Scene/Components.h"
#include "Sapphire/Scene/Entity.h"
#include "Sapphire/Utilities/FileIO.h"
#include "Sapphire/Renderer/Texture.h"

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}

	};
	template<>
	struct convert<glm::i32vec2>
	{
		static Node encode(const glm::i32vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::i32vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;
			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();
			return true;
		}
	};
}

namespace sph
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::i32vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << std::to_string(entity);

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNear();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFar();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.IsPrimary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.IsFixedAspectRatio;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();

			out << YAML::Key << "TextureName" << YAML::Value << spriteRenderer.Texture->GetPath();
			out << YAML::Key << "Color" << YAML::Value << spriteRenderer.Color;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteAnimatorComponent>())
		{
			out << YAML::Key << "SpriteAnimatorComponent";
			out << YAML::BeginMap;

			auto& spriteAnimator = entity.GetComponent<SpriteAnimatorComponent>();
			out << YAML::Key << "FrameCount" << YAML::Value << spriteAnimator.FrameCount;
			out << YAML::Key << "FrameTime" << YAML::Value << spriteAnimator.FrameTime;
			out << YAML::Key << "IsLooping" << YAML::Value << spriteAnimator.IsLooping;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
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

		auto path = _filepath;
		if (FileIO::GetFileExtension(_filepath) != SPH_SCENE_FILE_EXTENSION)
		{
			path += ".sph";
		}

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_scene->GetName();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		for (auto entityID : m_scene->m_registry.view<entt::entity>())
		{
			Entity entity = { entityID, m_scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream outFile(path);
		outFile << out.c_str();

		LogDebug("SceneSerializer: Saved Scene '{0}' to '{1}'", m_scene->GetName(), path);
	}

	bool SceneSerializer::Deserialize(const std::string& _filepath)
	{
		if (m_scene == nullptr)
		{
			LogError("SceneSerializer: Scene is null");
			return false;
		}
		if (_filepath.empty())
		{
			LogError("SceneSerializer: Filepath is empty");
			return false;
		}
		if (FileIO::GetFileExtension(_filepath) != SPH_SCENE_FILE_EXTENSION)
		{
			LogError("SceneSerializer: Filepath is not a scene file");
			return false;
		}

		YAML::Node data = YAML::LoadFile(_filepath);

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		m_scene->SetName(sceneName);

		auto entities = data["Entities"];
		if (entities.IsNull())
		{
			LogError("SceneSerializer: No entities found in scene file");
			return false;
		}

		for (auto entity : entities)
		{
			uint64_t uuid = entity["Entity"].as<uint64_t>();

			std::string name;
			auto tagComponent = entity["TagComponent"];
			if (tagComponent)
			{
				name = tagComponent["Tag"].as<std::string>();
			}

			Entity deserializedEntity = m_scene->CreateEntity(name);

			auto transformComponent = entity["TransformComponent"];
			if (transformComponent)
			{
				auto& tc = deserializedEntity.GetComponent<TransformComponent>();
				tc.Translation = transformComponent["Translation"].as<glm::vec3>();
				tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
				tc.Scale = transformComponent["Scale"].as<glm::vec3>();
			}

			auto cameraComponent = entity["CameraComponent"];
			if (cameraComponent)
			{
				auto& cc = deserializedEntity.AddComponent<CameraComponent>();

				auto cameraProps = cameraComponent["Camera"];
				cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

				cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
				cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
				cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

				cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
				cc.Camera.SetOrthographicNear(cameraProps["OrthographicNear"].as<float>());
				cc.Camera.SetOrthographicFar(cameraProps["OrthographicFar"].as<float>());

				cc.IsPrimary = cameraComponent["Primary"].as<bool>();
				cc.IsFixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
			}

			auto spriteRendererComponent = entity["SpriteRendererComponent"];
			if (spriteRendererComponent)
			{
				auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
				if (spriteRendererComponent["TextureName"])
				{
					auto path = spriteRendererComponent["TextureName"].as<std::string>();
					src.Texture = Texture2D::Create(path);
				}
				src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
			}

			auto spriteAnimatorComponent = entity["SpriteAnimatorComponent"];
			if (spriteAnimatorComponent)
			{
				auto& sac = deserializedEntity.AddComponent<SpriteAnimatorComponent>();
				sac.FrameCount = spriteAnimatorComponent["FrameCount"].as<glm::i32vec2>();
				sac.FrameTime = spriteAnimatorComponent["FrameTime"].as<float>();
				sac.IsLooping = spriteAnimatorComponent["IsLooping"].as<bool>();
			}
		}

		LogDebug("SceneSerializer: Loaded Scene '{0}' from '{1}'", sceneName, _filepath);

		return true;
	}
}