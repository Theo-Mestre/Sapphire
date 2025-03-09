#include "sphpch.h"
#include "FileIO.h"

#include "Sapphire/Core/Core.h"
#include "Sapphire/Platform/Windows/WindowsFileIO.h"

namespace sph
{
    std::optional<std::string> FileIO::OpenFile(const char* filter)
    {
#ifdef SPH_PLATFORM_WINDOWS
		return WindowsFileIO::OpenFile(filter);
#endif
		return std::nullopt;
    }

    std::optional<std::string> FileIO::SaveFile(const char* filter)
    {
#ifdef SPH_PLATFORM_WINDOWS
        return WindowsFileIO::SaveFile(filter);
#endif
		return std::nullopt;
    }

    std::string FileIO::GetFileExtension(const std::string& _filepath)
    {
		size_t pos = _filepath.find_last_of('.');
		return pos == std::string::npos ? "" : _filepath.substr(pos + 1);
    }
}