#include <XH.h>

class ExampleLayer : public XH::Layer
{
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override
	{
		XH_INFO("Example Layer::Update");
	}

	void OnEvent(XH::Event& event) override
	{
		XH_TRACE("{0}", event);
	}
};


class Examples : public XH::Application
{
public:
	Examples()
	{
		PushLayer(new ExampleLayer());
	}

	~Examples()
	{

	}
};


XH::Application* XH::CreateApplication()
{
	return new Examples();
}
