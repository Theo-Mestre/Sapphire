#pragma once

#include "Sapphire/Core/Core.h"

namespace sph
{
	class Scene;

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& _scene);
		void Serialize(const std::string& _filepath);
		bool Deserialize(const std::string& _filepath);

	private:
		Ref<Scene> m_scene;
	};
}