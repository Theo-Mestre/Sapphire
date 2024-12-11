#ifndef SPH_SCENE_H
#define SPH_SCENE_H

#include <string>
#include <entt/entt.hpp>

#include "Sapphire/Core/Core.h"
#include "Sapphire/Core/Time.h"

namespace sph
{
	class Renderer;
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(DeltaTime _dt);
		void OnRender(const Ref<Renderer>& _renderer);

		Entity CreateEntity(const std::string& _name = "");

		entt::registry& Registry() { return m_registry; }
	private:
		entt::registry m_registry;

		friend class Entity;
	};
}
#endif