#include <glad/glad.h>
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

#include "JuliaService.h"
#include <iostream>
#include <vector>

#include "../API/GL_Shader.h"
#include "../Backend/Backend.h"

extern "C" void LaunchJuliaKernel(unsigned char* devPtr, int width, int height, float rc, float rs);

namespace JuliaService
{
    unsigned int VAO, VBO;
    unsigned int textureID;
    unsigned int pboID;
    struct cudaGraphicsResource* cudaPboResource = nullptr;

    Shader* juliaShader;

    int cachedWidth = -1;
    int cachedHeight = -1;

    bool isPaused = false;
    float alpha = 1.57f;
    float currentRc = 0.0f;
    float currentRs = 0.0f;

    void Init()
    {

        float vertices[] = {
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        glGenTextures(1, &textureID);
        glGenBuffers(1, &pboID);

        juliaShader = new Shader("C:/Users/mijiy/source/repos/cpp/JuliaSetCpp/res/Shaders/julia.vs", "C:/Users/mijiy/source/repos/cpp/JuliaSetCpp/res/Shaders/julia.fs");
        juliaShader->use();
        glUniform1i(glGetUniformLocation(juliaShader->ID, "screenTexture"), 0);
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

            if (cudaPboResource != nullptr)
            {
                cudaGraphicsUnregisterResource(cudaPboResource);
            }

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboID);
            glBufferData(GL_PIXEL_UNPACK_BUFFER, newWidth * newHeight * 3 * sizeof(unsigned char), NULL, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

            cudaGraphicsGLRegisterBuffer(&cudaPboResource, pboID, cudaGraphicsMapFlagsWriteDiscard);

            //glViewport(0, 0, newWidth, newHeight);
        }
    }

    void Render()
    {
        if (cachedWidth <= 0 || cachedHeight <= 0 || cudaPboResource == nullptr) return;

        if (!isPaused)
        {
            alpha += 0.01f;
            if (alpha > 6.28318f) alpha -= 6.28318f;
        }

        currentRc = 0.7885f * cos(alpha);
        currentRs = 0.7885f * sin(alpha);

        unsigned char* devPtr;
        size_t size;
        float time = (float)Backend::GetTime();

        cudaGraphicsMapResources(1, &cudaPboResource, NULL);
        cudaGraphicsResourceGetMappedPointer((void**)&devPtr, &size, cudaPboResource);

        LaunchJuliaKernel(devPtr, cachedWidth, cachedHeight, currentRc, currentRs);

        cudaGraphicsUnmapResources(1, &cudaPboResource, NULL);

        juliaShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboID);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, cachedWidth, cachedHeight, GL_RGB, GL_UNSIGNED_BYTE, 0);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void Destroy()
    {
        cudaGraphicsUnregisterResource(cudaPboResource);
        glDeleteBuffers(1, &pboID);
        glDeleteTextures(1, &textureID);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        delete juliaShader;
    }

    bool& GetPausedState()
    {
        return isPaused;
    }

    float& GetAlphaState()
    {
        return alpha;
    }

    float GetCurrentRc()
    {
        return currentRc;
    }
    float GetCurrentRs()
    {
        return currentRs;
    }
}