#include <XH.h>

class Examples : public XH::Application
{
public:
	Examples()
	{

	}

	~Examples()
	{

	}
};


XH::Application* XH::CreateApplication()
{
	return new Examples();
}
