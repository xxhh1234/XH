#pragma once

#include "XH/Renderer/GraphicsContext.h"

#include <glad/glad.h>

struct GLFWwindow;

namespace XH
{

    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);

        virtual void Init() override;
        virtual void SwapBuffers() override;
    private:
        GLFWwindow* m_WindowHandle;
    };

}
