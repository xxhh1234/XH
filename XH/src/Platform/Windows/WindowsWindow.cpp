#include "xhpch.h"
#include "WindowsWindow.h"

namespace XH
{
    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        XH_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    XH::WindowsWindow::~WindowsWindow()
    {
        ShutDown();
    }

    void XH::WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void XH::WindowsWindow::SetVSync(bool enabled)
    {
        glfwSwapInterval(enabled);
        m_Data.VSync = enabled;
    }

    bool XH::WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }

    void XH::WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        XH_CORE_INFO("Creating Window {0} ({1},{2})", m_Data.Title, m_Data.Width, m_Data.Height);

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            XH_CORE_ASSERT(success, "Could not Initialized GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(static_cast<int>(m_Data.Width), static_cast<int>(m_Data.Height),
                                    m_Data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Set GLFW Callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent e1(key, 0);
                    data.EventCallback(e1);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent e2(key);
                    data.EventCallback(e2);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent e3(key, 1);
                    data.EventCallback(e3);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent e1(button);
                    data.EventCallback(e1);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonPressedEvent e2(button);
                    data.EventCallback(e2);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
            data.EventCallback(event);
        });
    }

    void XH::WindowsWindow::ShutDown()
    {
        glfwDestroyWindow(m_Window);
    }
}
