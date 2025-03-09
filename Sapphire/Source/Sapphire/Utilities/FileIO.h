#ifndef SPH_FILE_IO_H
#define SPH_FILE_IO_H

#include <string>

namespace sph
{
	class FileIO
	{
	public:
		static std::optional<std::string> OpenFile(const char* filter);
		static std::optional<std::string> SaveFile(const char* filter);

		static std::string GetFileExtension(const std::string& _filepath);
	};
}

#endif