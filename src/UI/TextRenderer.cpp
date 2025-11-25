#include "TextRenderer.h"
#include <map>
#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../API/GL_Shader.h"
#include "../misc/filesystem.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../Backend/Backend.h"

namespace TextRenderer
{
	const float REFERENCE_WIDTH = 1920.0f;
	const float REFERENCE_HEIGHT = 1080.0f;
	const unsigned int ARRAY_LIMIT = 128;

	/// Holds all state information relevant to a character as loaded using FreeType
	struct Character
	{
		int TextureID; // ID handle of the glyph texture
		glm::ivec2   Size;      // Size of glyph
		glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
		unsigned int Advance;   // Horizontal offset to advance to next glyph
	};

	std::map<GLchar, Character> Characters;
	unsigned int VAO, VBO;
	GLuint textureArray;
	std::vector<glm::mat4> transforms;
	std::vector<int> letterMap;
	Shader* textShader;

	static glm::mat4 s_projection;
	int cachedWidth = -1;
	int cachedHeight = -1;

	void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);
	void TextRenderCall(int length, GLuint shader);

	bool Init()
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			return false;
		}

		std::string fontRoot = FileSystem::getPath("res/Fonts/Born2bSportyFS.otf");

		std::cout << "Attempting to load font from: " << fontRoot << std::endl;

		FT_Face face;
		if (FT_New_Face(ft, fontRoot.c_str(), 0, &face))
		{
			std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
			return false;
		}
		else
		{
			FT_Set_Pixel_Sizes(face, 0, 256);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &textureArray);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, 256, 256, 128, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

			for (unsigned char c = 0; c < 128; c++)
			{
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					std::cout << "ERROR::FREETYTPE: Failed to load Glyph " << c << std::endl;
					continue;
				}
				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, int(c), face->glyph->bitmap.width, face->glyph->bitmap.rows, 1, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

				Character character = {
					int(c),
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					static_cast<unsigned int>(face->glyph->advance.x)
				};
				Characters.insert(std::pair<char, Character>(c, character));
			}
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		}
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		for (int i = 0; i < ARRAY_LIMIT; i++)
		{
			letterMap.push_back(0);
			transforms.push_back(glm::mat4(1.0f));
		}

		GLfloat vertex_data[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		textShader = new Shader("C:/Users/mijiy/source/repos/cpp/JuliaSetCpp/res/Shaders/text.vs", "C:/Users/mijiy/source/repos/cpp/JuliaSetCpp/res/Shaders/text.fs");

		textShader->use();
		glUniform1i(glGetUniformLocation(textShader->ID, "ourTexture"), 0);

		return true;
	}

	void BeginFrame()
	{
		int newWidth = Backend::GetCurrentWindowWidth();
		int newHeight = Backend::GetCurrentWindowHeight();

		if (newWidth <= 0 || newHeight <= 0) return;

		if (newWidth != cachedWidth || newHeight != cachedHeight)
		{
			cachedWidth = newWidth;
			cachedHeight = newHeight;

			glViewport(0, 0, newWidth, newHeight);

			s_projection = glm::ortho(0.0f, static_cast<float>(newWidth), static_cast<float>(newHeight), 0.0f);

			textShader->use();
			glUniformMatrix4fv(glGetUniformLocation(textShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(s_projection));
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Render(const std::string& text, float x, float y, float scale, const glm::vec3& color)
	{
		if (cachedWidth <= 0 || cachedHeight <= 0) return;

		float widthRatio = (float)cachedWidth / REFERENCE_WIDTH;
		float heightRatio = (float)cachedHeight / REFERENCE_HEIGHT;

		float finalX = x * widthRatio;
		float finalY = y * heightRatio;
		float copyX = finalX;

		float finalScale = scale * heightRatio * (48.0f / 256.0f);

		textShader->use();
		glUniform3f(glGetUniformLocation(textShader->ID, "textColor"), color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		int workingIndex = 0;

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];

			if (*c == '\n')
			{
				finalY -= ((ch.Size.y)) * 1.3 * finalScale;
				finalX = copyX;
			}
			else if (*c == ' ')
			{
				finalX += (ch.Advance >> 6) * finalScale;
			}
			else
			{
				float xpos = finalX + ch.Bearing.x * finalScale;
				float ypos = finalY - (ch.Size.y - ch.Bearing.y) * finalScale;

				transforms[workingIndex] = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(256 * finalScale, 256 * finalScale, 0));
				letterMap[workingIndex] = ch.TextureID;

				finalX += (ch.Advance >> 6) * finalScale;
				workingIndex++;
				if (workingIndex == ARRAY_LIMIT)
				{
					TextRenderCall(workingIndex, textShader->ID);
					workingIndex = 0;
				}
			}
		}
		TextRenderCall(workingIndex, textShader->ID);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}

	void Destroy()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteTextures(1, &textureArray);
	}

	void TextRenderCall(int length, GLuint shader)
	{
		if (length != 0)
		{
			glUniformMatrix4fv(glGetUniformLocation(shader, "transforms"), length, GL_FALSE, &transforms[0][0][0]);
			glUniform1iv(glGetUniformLocation(shader, "letterMap"), length, &letterMap[0]);
			glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, length);
		}

	}
}