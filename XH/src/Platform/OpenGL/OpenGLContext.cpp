#include "xhpch.h"

#include "OpenGLContext.h"
#include <GLFW/glfw3.h>

namespace XH
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        :m_WindowHandle(windowHandle)
    {
        XH_PROFILE_FUNCTION();

        XH_CORE_ASSERT(windowHandle, "windowHandle is null");
    }
    void OpenGLContext::Init()
    {
        XH_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);

        int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        XH_CORE_ASSERT(status, "Failed to initialized Glad!");
    }
    void OpenGLContext::SwapBuffers()
    {
        XH_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }
}