#include "xhpch.h"
#include "WindowsInput.h"

#include "XH/Core/Application.h"
#include <GLFW/glfw3.h>

namespace XH
{
    Input* Input::s_instance = new WindowsInput();

    bool WindowsInput::IsKeyPressedImpl(int keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }
    bool WindowsInput::IsMouseButtonPressedImpl(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }
    float WindowsInput::GetMouseXImpl()
    {
        auto [x, y] = GetMousePos();
        return x;
    }
    float WindowsInput::GetMouseYImpl()
    {
        auto [x, y] = GetMousePos();
        return y;
    }
    std::pair<float, float> WindowsInput::GetMousePosImpl()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return { static_cast<float>(xpos), static_cast<float>(ypos) };
    }
}

