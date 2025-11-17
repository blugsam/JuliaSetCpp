#include "UserInput.h"

namespace UserInput
{
    bool keyPressed[372];
    bool keyDown[372];
    bool keyDownLastFrame[372];
    GLFWwindow* window;

    void Init(void* glfwWindow)
    {
        window = static_cast<GLFWwindow*>(glfwWindow);
    }

    void Update()
    {
        for (int i = 32; i < 349; i++)
        {
            // down
            if (glfwGetKey(window, i) == GLFW_PRESS)
                keyDown[i] = true;
            else
                keyDown[i] = false;

            // press
            if (keyDown[i] && !keyDownLastFrame[i])
                keyPressed[i] = true;
            else
                keyPressed[i] = false;
            keyDownLastFrame[i] = keyDown[i];
        }
    }

    void ClearKeyStates()
    {
        for (int i = 32; i < 349; i++)
        {
            keyDown[i] = false;
            keyPressed[i] = false;
            keyDownLastFrame[i] = false;
        }
    }

    bool KeyPressed(unsigned int keycode)
    {
        return keyPressed[keycode];
    }

    bool KeyDown(unsigned int keycode)
    {
        return keyDown[keycode];
    }
}