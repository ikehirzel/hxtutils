#include <hirzel/path/Path.hpp>

#ifdef _WIN32
#include <Windows.h>
#define DEFAULT_SEPARATOR	('\\')
#define PATH_SEPARATOR		(':')
#define USER_HOME_DIR_ENV_VAR	"USERPROFILE"
#define CONFIG_DIR_PATH			"\\AppData\\Roaming"
#else
#define DIRECTORY_SEPARATOR	('/')
#define PATH_SEPARATOR		(';')
#define USER_HOME_DIR_ENV_VAR "HOME"
#define CONFIG_DIR_PATH			"/.config"
#endif

namespace hirzel::path
{
	const char directorySeparator = DEFAULT_SEPARATOR;
	const char pathSeparator = PATH_SEPARATOR;

	const char* homeDirectoryPath()
	{
		thread_local static char path[256];

		if (path[0] == '\0')
		{
			auto* value = getenv(USER_HOME_DIR_ENV_VAR);

			strcpy(path, value);
		}

		return path;
	}

	const char* configDirectoryPath()
	{
		thread_local static char path[256];
		
		if (path[0] == '\0')
		{
			const auto* homeDirPath = homeDirectoryPath();

			strcpy(path, homeDirPath);
			strcat(path, CONFIG_DIR_PATH);
		}

		return path;
	}

	std::string executablePath()
	{
#ifdef _WIN32

		char buf[MAX_PATH + 1];
		GetModuleFileNameA(NULL, buf, MAX_PATH);
		return std::string(buf);

#elif defined(__linux__)

		char buf[512];
		readlink("/proc/self/exe", buf, sizeof(buf) / sizeof(char) - 1);
		return std::string(buf);

#else
		throw std::runtime_error("Unable to get executable path for OS: " + std::string(system::currentOsName));
#endif
	}

	std::string concatenate(const std::initializer_list<const char*> paths)
	{
		auto path = Path();

		for (const auto* p : paths)
			path.append(p);

		return path.toString();
	}

	std::string parentOf(const char* path)
	{
		return Path(path).parent();
	}

	std::string filenameOf(const char* path, bool includeExtension)
	{
		return Path(path).filename(includeExtension);
	}

	std::string extensionOf(const char* path)
	{
		return Path(path).extension();
	}
}