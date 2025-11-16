#include <GLFW/glfw3.h>
#include <cuda.h>
#include <cuda_runtime.h>

#include "glfwErrorHandler.h"
#include "WindowInit.h"
#include "KeyCallback.h"

int main(void)
{
	glfwSetErrorCallback(GlfwErrorHandler::GlfwErrorCallback);

	GLFWwindow* window = WindowInit::juliaSetCreateWindow();

	glfwSetKeyCallback(window, KeyCallback::FullscreenKeyCallback);

	window = glfwCreateWindow(DIM, DIM, "Julia set", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glViewport(0, 0, DIM, DIM);
	/* Создаём битовую карту */
	DataBlock data;
	unsigned char* bitmap = new unsigned char[DIM * DIM * 4];
	/* указатель dev bitmap для хранения копии данных на устройстве (GPU) */
	unsigned char* dev_bitmap;

	while (!glfwWindowShouldClose(window))
	{
		cudaMalloc((void**)&dev_bitmap, DIM * DIM * 4);
		data.dev_bitmap = dev_bitmap;
		dim3 grid(DIM, DIM);
		kernel << <grid, 1 >> > (dev_bitmap);
		cudaMemcpy(bitmap, dev_bitmap, DIM * DIM * 4, cudaMemcpyDeviceToHost);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawPixels(DIM, DIM, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	cudaFree(dev_bitmap);
	delete bitmap;
	return 0;
}
