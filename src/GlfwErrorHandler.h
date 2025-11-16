#pragma once

#include <iostream>

namespace GlfwErrorHandler
{
	static void GlfwErrorCallback(int error, const char* description)
	{
		std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
	}
}