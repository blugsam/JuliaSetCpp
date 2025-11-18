#pragma once
#include <string>
#include <glm/glm.hpp>

namespace TextRenderer
{
	bool Init();
	void BeginFrame();
	void Render(std::string text, float x, float y, float scale, glm::vec3 color);
	void Destroy();
}