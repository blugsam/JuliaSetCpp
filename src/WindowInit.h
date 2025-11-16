#include <iostream>
#include <GLFW/glfw3.h>

namespace WindowInit
{
	constexpr int MSAA_VALUE = 8;

	struct WindowState
	{
		bool fullScreen = true;
		int windowWidth = 800;
		int windowHeight = 600;
		int windowedXPos = 100;
		int windowedYPos = 100;
	};

	GLFWwindow* juliaSetCreateWindow()
	{
		WindowState* state = new WindowState();

		if (!glfwInit())
		{
			std::cerr << "Failed to initialize GLFW" << std::endl;

			return nullptr;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, MSAA_VALUE);

		GLFWmonitor* monitor = nullptr;
		if (state->fullScreen)
		{
			monitor = glfwGetPrimaryMonitor();

			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			state->windowWidth = mode->width;
			state->windowWidth = mode->height;
		}

		GLFWwindow* window = glfwCreateWindow(state->windowWidth, state->windowHeight, "Julia set", monitor, NULL);
		if (!window)
		{
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();

			delete state;
			return nullptr;
		}

		glfwMakeContextCurrent(window);

		return window;
	}

	void framebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}