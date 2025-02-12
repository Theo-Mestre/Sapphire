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
	private:
		Entity m_Entity;
		friend class Scene;
	};
}
#endif