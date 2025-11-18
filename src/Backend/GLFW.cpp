#include "GLFW.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <iostream>
#include <string>

namespace Backend::GLFW
{
    const int MSAA_VALUE = 0;
    const int XWINDOW_POS = 5;
    const int YWINDOW_POS = 35;
    const GLFWvidmode* MODE;
	GLFWwindow* window = nullptr;
	GLFWmonitor* monitor = nullptr;
	WindowedMode windowedMode = WindowedMode::WINDOWED;
    int windowedWidth = 0;
    int windowedHeight = 0;
    int fullscreenWidth = 0;
    int fullscreenHeight = 0;
    int currentWindowWidth = 0;
    int currentWindowHeight = 0;
    bool forceCloseWindow = false;

    void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

    bool Init(WindowedMode newWindowedMode)
    {
        glfwInit();

        glfwSetErrorCallback([](int error, const char* description) { std::cerr << "ERROR::GLFW: (" << error << "): "<< description << std::endl; });

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, MSAA_VALUE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);

        monitor = glfwGetPrimaryMonitor();
        MODE = glfwGetVideoMode(monitor);
        glfwWindowHint(GLFW_RED_BITS, MODE->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, MODE->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, MODE->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, MODE->refreshRate);
        // hide windows frame
        /*glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);*/
        fullscreenWidth = MODE->width;
        fullscreenHeight = MODE->height;
        windowedWidth = fullscreenWidth * 0.75f;
        windowedHeight = fullscreenHeight * 0.75f;

        windowedMode = newWindowedMode;
        if (windowedMode == WindowedMode::WINDOWED)
        {
            currentWindowWidth = windowedWidth;
            currentWindowHeight = windowedHeight;
            window = glfwCreateWindow(windowedWidth, windowedHeight, "Julia set", nullptr, nullptr);
            glfwSetWindowPos(window, XWINDOW_POS, YWINDOW_POS);
        }
        else if (windowedMode == WindowedMode::FULLSCREEN)
        {
            currentWindowWidth = fullscreenWidth;
            currentWindowHeight = fullscreenHeight;
            window = glfwCreateWindow(fullscreenWidth, fullscreenHeight, "Julia set", nullptr, nullptr);
        }
        if (window == nullptr)
        {
            std::cerr << "ERROR::GLFW: Failed to create window." << std::endl;
            glfwTerminate();
            return false;
        }
        glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

        return true;
    }

    void MakeContextCurrent()
    {
        glfwMakeContextCurrent(window);
    }

    void BeginFrame()
    {
        glfwPollEvents();
    }

    void EndFrame()
    {
        glfwSwapBuffers(window);
    }

    void Destroy()
    {
        glfwTerminate();
    }

    void SetWindowedMode(const WindowedMode& newWindowedMode)
    {
        windowedMode = newWindowedMode;

        if (windowedMode == WindowedMode::FULLSCREEN)
        {
            currentWindowWidth = fullscreenWidth;
            currentWindowHeight = fullscreenHeight;
        }
        else if (windowedMode == WindowedMode::WINDOWED)
        {
            currentWindowWidth = windowedWidth - 1;
            currentWindowHeight = windowedHeight - 1;
        }

        glfwSetWindowMonitor(window, nullptr, 0, 0, currentWindowWidth, currentWindowHeight, MODE->refreshRate);
        glfwSetWindowPos(window, XWINDOW_POS, YWINDOW_POS);
    }

    void ToggleFullscreen()
    {
        if (windowedMode == WindowedMode::WINDOWED)
        {
            SetWindowedMode(WindowedMode::FULLSCREEN);
        }
        else
        {
            SetWindowedMode(WindowedMode::WINDOWED);
        }
    }

    void ForceCloseWindow()
    {
        forceCloseWindow = true;
    }

    bool WindowIsOpen()
    {
        return !(glfwWindowShouldClose(window) || forceCloseWindow);
    }

    void* GetWindowPointer()
    {
        return window;
    }

    int GetWindowedWidth()
    {
        return windowedWidth;
    }

    int GetWindowedHeight()
    {
        return windowedHeight;
    }

    int GetFullScreenWidth()
    {
        return fullscreenWidth;
    }

    int GetFullScreenHeight()
    {
        return fullscreenHeight;
    }

    int GetCurrentWindowWidth()
    {
        return currentWindowWidth;
    }

    int GetCurrentWindowHeight()
    {
        return currentWindowHeight;
    }

    double GetTime()
    {
        return glfwGetTime();
    }

    void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        // body
    }

}