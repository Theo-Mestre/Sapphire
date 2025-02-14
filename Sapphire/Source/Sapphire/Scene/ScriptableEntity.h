#ifndef SPH_SCRIPTABLE_ENTITY_H
#define SPH_SCRIPTABLE_ENTITY_H

#include "Entity.h"

namespace sph 
{
	class ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(DeltaTime _dt) {}

	protected:
		Entity m_Entity;
		friend class Scene;
	};
}
#endif