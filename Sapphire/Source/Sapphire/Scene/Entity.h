#ifndef SPH_ENTITY_H
#define SPH_ENTITY_H

#include "Sapphire/Core/Core.h"
#include "Sapphire/Scene/Scene.h"
#include "Sapphire/Core/Log.h"

namespace sph
{
	class Entity
	{
	public:
		Entity();
		Entity(entt::entity _handle, Scene* const _scene);
		Entity(const Entity&) = default;
		~Entity() = default;


		template<typename T, typename... Args>
		T& AddComponent(Args&&... _args)
		{
			ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(_args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_scene->m_registry.get<T>(m_handle);
		}

		template<typename T>
		bool HasComponent()
		{
			ASSERT(m_scene != nullptr, "Scene is expired!");
			return m_scene->m_registry.all_of<T>(m_handle);
		}

		template<typename T>
		void RemoveComponent()
		{
			ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_scene->m_registry.remove<T>(m_handle);
		} 

		bool IsValid() const;
		entt::entity GetHandle() const;

		operator bool() const { return m_handle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_handle; }
		bool operator==(const Entity& _other) const;
		bool operator!=(const Entity& _other) const;

		static Entity Create(const Ref<Scene>& _scene, const char* _name = "");

	private:
		entt::entity m_handle;
		Scene* m_scene = nullptr;
	};
}
#endif