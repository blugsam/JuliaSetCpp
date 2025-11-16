#include <GLFW/glfw3.h>

#include "WindowInit.h"

namespace KeyCallback
{
    void FullscreenKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action != GLFW_PRESS) return;

        WindowInit::WindowState* state = static_cast<WindowInit::WindowState*>(glfwGetWindowUserPointer(window));
        if (!state) return;

        if (key == GLFW_KEY_ESCAPE && mods == 0) glfwSetWindowShouldClose(window, GLFW_TRUE);

        if (key == GLFW_KEY_F11 && mods == 0)
        {
            if (glfwGetWindowMonitor(window))
            {
                glfwSetWindowMonitor(window, NULL, state->windowedXPos, state->windowedYPos, state->windowWidth, state->windowHeight, 0);
            }
            else
            {
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                if (monitor)
                {
                    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                    glfwGetWindowPos(window, &state->windowedXPos, &state->windowedYPos);
                    glfwGetWindowSize(window, &state->windowWidth, &state->windowHeight);
                    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                }
            }
        }
    }
}