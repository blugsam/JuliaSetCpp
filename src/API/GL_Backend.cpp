#include "GL_backend.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../Backend/GLFW.h"

namespace OpenGLBackend
{
	bool Init()
	{
		Backend::GLFW::MakeContextCurrent();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "Failed to initialize GLAD" << std::endl;
			return false;
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Print some shit
		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		const GLubyte* vendor = glGetString(GL_VENDOR);
		const GLubyte* renderer = glGetString(GL_RENDERER);
		std::cout << "\nGPU: " << renderer << std::endl;
		std::cout << "GL version: " << major << "." << minor << "\n\n";

		return true;
	}

	void BeginFrame()
	{
		glClearColor(0.2f, 0.1f, 0.3f, 0.4f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}