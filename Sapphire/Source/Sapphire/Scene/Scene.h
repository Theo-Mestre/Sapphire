#ifndef SPH_SCENE_H
#define SPH_SCENE_H

#include <string>
#include <entt/entt.hpp>

#include "Sapphire/Core/Core.h"
#include "Sapphire/Core/Time.h"

namespace sph
{
	class EditorCamera;
	class Renderer;
	class Entity;

	class Scene
	{
	public:
		Scene();
		Scene(const std::string& _name);
		~Scene();

		void OnUpdateRuntime(DeltaTime _dt);
		void OnUpdateEditor(DeltaTime _dt);

		void OnRenderRuntime(const Ref<Renderer>& _renderer);
		void OnRenderEditor(const Ref<Renderer>& _renderer, const Ref<EditorCamera>& _camera);

		Entity CreateEntity(const std::string& _name = "");
		void DestroyEntity(Entity _entity);

		entt::registry& Registry() { return m_registry; }

		void OnViewportResize(uint32_t _width, uint32_t _height);

		Entity GetPrimaryCameraEntity();

		inline void SetName(const std::string& _name) { m_name = _name; }
		inline const std::string& GetName() const { return m_name; }
	private:
		template<typename T>
		void OnComponentAdded(Entity _entity, T& _component);
	private:
		entt::registry m_registry;
		std::string m_name;

		uint32_t m_viewportWidth = 0;
		uint32_t m_viewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}
#endif