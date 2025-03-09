#ifndef SPH_WINDOWS_FILE_IO_H
#define SPH_WINDOWS_FILE_IO_H

namespace sph
{
	class WindowsFileIO
	{
	public:
		static std::optional<std::string> OpenFile(const char* filter);
		static std::optional<std::string> SaveFile(const char* filter);

		static void SetHWND(void* _hwnd) { m_hwnd = _hwnd; }

	private:
		inline static void* m_hwnd;
	};
}

#endif