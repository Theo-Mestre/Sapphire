#ifndef SPH_PROPERTIES_PANEL_H
#define SPH_PROPERTIES_PANEL_H

#include "Sapphire/Core/Core.h"

namespace sph
{
	class SceneHierarchyPanel;

	class PropertiesPanel
	{
	public:
		PropertiesPanel(const Ref<SceneHierarchyPanel>& _context);
		~PropertiesPanel();

		void OnImGuiRender();
		
		void SetContext(const Ref<SceneHierarchyPanel>& _context);

	private:
		void DrawComponents(Entity _entity);

	private:
		Ref<SceneHierarchyPanel> m_context;
	};
}
#endif