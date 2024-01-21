#include <XH.h>
/// -------Entry Point-------
#include "XH/Core/EntryPoint.h"

#include "ExampleLayer.h"
#include "Renderer2DLayer.h"

class ExampleApp : public XH::Application
{
public:
    ExampleApp()
    {
        // PushLayer(new ExampleLayer());
        PushOverlay(new Renderer2DLayer());
    }

    ~ExampleApp()
    {

    }
};

XH::Application* XH::CreateApplication()
{
    return new ExampleApp();
}
