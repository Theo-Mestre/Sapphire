#ifndef SPH_ENTRYPOINT_H
#define SPH_ENTRYPOINT_H

#include "Sapphire/Profiling/FrameTimeData.h"

#include "Application.h"

extern sph::Application* sph::CreateApplication();

int main(int argc, char** argv)
{
	SPH_INIT_PROFILING();

	SPH_PROFILE_BEGIN_SESSION("Startup", "Startup.json");
	auto app = sph::CreateApplication();
	SPH_PROFILE_END_SESSION();

	SPH_PROFILE_BEGIN_SESSION("Runtime", "Runtime.json");
	app->Run();
	SPH_PROFILE_END_SESSION();

	SPH_PROFILE_BEGIN_SESSION("Shutdown", "Shutdown.json");
	delete app;
	SPH_PROFILE_END_SESSION();

	SPH_SHUTDOWN_PROFILING();
	return 0;
}

#endif