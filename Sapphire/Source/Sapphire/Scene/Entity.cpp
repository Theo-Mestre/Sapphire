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
		: m_handle(_handle), m_scene(_scene)
	{
	}

	Entity Entity::Create(const Ref<Scene>& _scene, const char* _name)
	{
		return _scene->CreateEntity(_name);
	}
}