#ifndef SPH_ENTRYPOINT_H
#define SPH_ENTRYPOINT_H

#include "Application.h"

extern sph::Application* sph::CreateApplication();

int main(int argc, char** argv)
{
	//PROFILE_BEGIN_SESSION("Startup");
	auto app = sph::CreateApplication();
	//PROFILE_END_SESSION();

	//PROFILE_BEGIN_SESSION("Runtime");
	app->Run();
	//PROFILE_END_SESSION();

	//PROFILE_BEGIN_SESSION("Shutdown");
	delete app;
	//PROFILE_END_SESSION();
	return 0;
}

#endif