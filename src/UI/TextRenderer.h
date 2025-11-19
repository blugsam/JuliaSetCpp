#pragma once
#include <string>
#include <glm/glm.hpp>

namespace TextRenderer
{
	bool Init();
	void BeginFrame();
	void Render(const std::string& text, float x, float y, float scale, const glm::vec3& color);
	void Destroy();
}