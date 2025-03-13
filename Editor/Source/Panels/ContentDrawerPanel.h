#ifndef SPH_CONTENT_DRAWER_H
#define SPH_CONTENT_DRAWER_H

#include <filesystem>

#include "Sapphire/Core/Core.h"

namespace sph
{
	class Texture2D;

	class ContentDrawerPanel
	{
	public:
		ContentDrawerPanel();
		~ContentDrawerPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_currentDirectory;

		Ref<Texture2D> m_directoryIcon;
		Ref<Texture2D> m_fileIcon;
	};
}
#endif