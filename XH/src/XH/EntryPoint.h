#pragma once

#ifdef XH_PLATFORM_WINDOWS

#include "stdio.h"

extern XH::Application* XH::CreateApplication();

int main(int argc, char** argv)
{
	printf("Hello World!");
	auto app = XH::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif


