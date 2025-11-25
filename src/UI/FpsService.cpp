#include "FpsService.h"
#include <string>

#include "../Backend/GLFW.h"
#include "TextRenderer.h"
#include "../ViewPort/Viewport.h"
#include "../ViewPort/ViewPortService.h"

namespace FpsService
{
	std::string fps = "";
	double previousTime = 0;
	int frameCount = 0;

	void Update()
	{
		double currentTime = Backend::GLFW::GetTime();
		frameCount++;
		if (currentTime - previousTime >= 1.0)
		{
			fps = std::to_string(frameCount) + " FPS";

			frameCount = 0;
			previousTime = currentTime;
		}
	}

	void Render()
	{
		TextRenderer::Render(fps, 1800.0f, 10.0f, 1.0f, glm::vec3(1.0f));
	}
}