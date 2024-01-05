#pragma once

#ifdef XH_PLATFORM_WINDOWS

extern XH::Application* XH::CreateApplication();

int main(int argc, char** argv)
{
	XH::Log::Init();

	auto app = XH::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif


