#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "API/OpenGL/GL_Shader.h"
#include "misc/filesystem.h"

namespace TextRendering
{
    void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color, );
    void TextRenderCall(int length, GLuint shader);

    const unsigned int ARRAY_LIMIT = 400;

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

    void TextRender()
    {
        Shader shader("text.vs", "text.fs");
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
        shader.use();
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }

        std::string font_name = FileSystem::getPath("resources/fonts/Antonio-Bold.ttf");
        if (font_name.empty())
        {
            std::cerr << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        }

        // load font as face
        FT_Face face;
        if (FT_New_Face(ft, font_name.c_str(), 0, &face))
        {
            std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
        }
        else
        {
            // set size to load glyphs as
            FT_Set_Pixel_Sizes(face, 256, 256);

            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glGenTextures(1, &textureArray);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
            glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, 256, 256, 128, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            {
                // Load character glyph 
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }
                glTexSubImage3D(
                    GL_TEXTURE_2D_ARRAY,
                    0, 0, 0, int(c),
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows, 1,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // now store character for later use
                Character character = {
                    int(c),
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                Characters.insert(std::pair<char, Character>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        }
        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        for (int i = 0; i < ARRAY_LIMIT; i++)
        {
            letterMap.push_back(0);
            transforms.push_back(glm::mat4(1.0f));
        }


        GLfloat vertex_data[] = {
            0.0f,1.0f,
            0.0f,0.0f,
            1.0f,1.0f,
            1.0f,0.0f,
        };

        // configure VAO/VBO for texture quads
        // -----------------------------------
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }


    // render line of text
    // -------------------
    void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
    {
        // activate corresponding render state	
        scale = scale * 48.0f / 256.0f;
        float copyX = x;
        shader.use();
        glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        int workingIndex = 0;
        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];

            if (*c == '\n')
            {
                y -= ((ch.Size.y)) * 1.3 * scale;
                x = copyX;
            }
            else if (*c == ' ')
            {
                x += (ch.Advance >> 6) * scale;
            }
            else
            {
                float xpos = x + ch.Bearing.x * scale;
                float ypos = y - (256 - ch.Bearing.y) * scale;

                transforms[workingIndex] = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(256 * scale, 256 * scale, 0));
                letterMap[workingIndex] = ch.TextureID;
                //float w = ch.Size.x * scale;
                //float h = ch.Size.y * scale;
                // update VBO for each character
                /*float vertices[6][4] = {
                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos,     ypos,       0.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },

                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },
                    { xpos + w, ypos + h,   1.0f, 0.0f }
                };*/
                // render glyph texture over quad
                // update content of VBO memory
                //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

                // render quad
                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
                workingIndex++;
                if (workingIndex == ARRAY_LIMIT)
                {
                    TextRenderCall(workingIndex, shader.ID);
                    workingIndex = 0;
                }
            }
        }
        TextRenderCall(workingIndex, shader.ID);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
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
