#include "sphpch.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "WindowsFileIO.h"

#include "Sapphire/Core/Log.h"
#include "Sapphire/Utilities/FileIO.h"

namespace sph
{
	std::optional<std::string> WindowsFileIO::OpenFile(const char* _filter)
	{
		if (m_hwnd == nullptr)
		{
			LogError("WindowsFileIO: HWND is null");
			return std::nullopt;
		}

		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };

		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)m_hwnd);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
		{
			ofn.lpstrInitialDir = currentDir;
		}

		ofn.lpstrFilter = _filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::nullopt;
	}

	std::optional<std::string> WindowsFileIO::SaveFile(const char* _filter)
	{
		if (m_hwnd == nullptr)
		{
			LogError("WindowsFileIO: HWND is null");
			return std::nullopt;
		}

		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };

		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)m_hwnd);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectoryA(256, currentDir))
		{
			ofn.lpstrInitialDir = currentDir;
		}
		ofn.lpstrFilter = _filter;
		ofn.nFilterIndex = 1;

		std::string fileExtension = FileIO::GetFileExtension(szFile);
		ofn.lpstrDefExt = fileExtension.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::nullopt;
	}
}