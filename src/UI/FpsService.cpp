#include "FpsService.h"

#include "../Backend/GLFW.h"
#include "TextRenderer.h"
#include <string>

// this is freaking insane
#include "../ViewPort/Viewport.h"
#include "../ViewPort/ViewPortService.h"

namespace FpsService
{
	std::string fps = "";
	double titleCountdownS = 0.1;
	double previousTime = 0;
	int frameCount = 0;
	Viewport* mainViewport = ViewportService::GetMainViewport();
	glm::mat4 projection = mainViewport->GetProjectionMatrix();

	void UpdateFps()
	{
		CalculateFps();
		PrintFps();
	}

	void CalculateFps()
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

	void PrintFps()
	{
		TextRenderer::Render(fps, 1000.0f, 500.0f, 0.5f, glm::vec3(1.0f));
	}
}