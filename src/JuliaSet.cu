#include <GLFW/glfw3.h>
#include <cuda.h>
#include <cuda_runtime.h>

#include "glfwErrorHandler.h"
#include "WindowInit.h"
#include "KeyCallback.h"

int main()
{
	glfwSetErrorCallback(GlfwErrorHandler::GlfwErrorCallback);

	GLFWwindow* window = WindowInit::JuliaSetCreateWindow();

	glfwSetFramebufferSizeCallback(window, WindowInit::framebufferSizeCallback);

	glfwSetKeyCallback(window, KeyCallback::FullscreenKeyCallback);

	std::cout << "System's CUDA supported version: " << glGetString(CUDA_VERSION) << std::endl;

	double prevS = glfwGetTime();
	double titleCountdownS = 0.1;

	while (!glfwWindowShouldClose(window))
	{
		double currS = glfwGetTime();
		double elapsedS = currS - prevS;
		prevS = currS;
		titleCountdownS -= elapsedS;
		if (titleCountdownS <= 0.0 && elapsedS > 0.0)
		{
			double fps = 1.0 / elapsedS;

			char tmp[256];
			sprintf(tmp, "FPS %.2lf", fps);
			glfwSetWindowTitle(window, tmp);
			titleCountdownS = 0.1;
		}
		
		glfwPollEvents();

		glClearColor(0.6f, 0.6f, 0.8f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapInterval(1);
		glfwSwapBuffers(window);
	}

	WindowInit::WindowState* state = static_cast<WindowInit::WindowState*>(glfwGetWindowUserPointer(window));
	delete state;

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}
