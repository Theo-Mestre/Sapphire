#ifndef SPH_SCENE_HIERARCHY_PANEL_H
#define SPH_SCENE_HIERARCHY_PANEL_H

#include "Sapphire/Core/Core.h"
#include "Sapphire/Scene/Entity.h"

namespace sph
{
	class Scene;

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(const Ref<Scene>& _context);
		~SceneHierarchyPanel();

		void OnImGuiRender();

		void SetContext(const Ref<Scene>& _context);
	private:
		void DrawEntityNode(Entity _entity);

	private:
		Ref<Scene> m_context;

		Entity m_selectedEntity;

		friend class PropertiesPanel;
	};
}
#endif