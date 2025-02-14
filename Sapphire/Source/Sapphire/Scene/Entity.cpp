#include "sphpch.h"
#include "Entity.h"

#include "Sapphire/Scene/Scene.h"

namespace sph
{
	Entity::Entity()
		: m_handle(entt::null)
		, m_scene(nullptr)
	{
	}

	Entity::Entity(entt::entity _handle, Scene* const _scene)
		: m_handle(_handle)
		, m_scene(_scene)
	{
	}

	bool Entity::IsValid() const
	{
		return m_scene != nullptr && m_scene->m_registry.valid(m_handle);
	}

	entt::entity Entity::GetHandle() const
	{
		return m_handle;
	}

	bool Entity::operator==(const Entity& _other) const
	{
		return m_handle == _other.m_handle && m_scene == _other.m_scene;
	}

	bool Entity::operator!=(const Entity& _other) const
	{
		return !(*this == _other);
	}

	Entity Entity::Create(const Ref<Scene>& _scene, const char* _name)
	{
		return _scene->CreateEntity(_name);
	}
}