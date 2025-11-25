#include "ImGuiService.h"
#include "../Backend/Backend.h"
#include "../JuliaSet/JuliaService.h"
#include "../UI/FpsService.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

namespace ImGuiService
{
    void Init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();

        GLFWwindow* window = static_cast<GLFWwindow*>(Backend::GetWindowPointer());
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Render()
    {
        ImGui::Begin("Julia Set Controls");

        bool& paused = JuliaService::GetPausedState();

        if (paused)
        {
            if (ImGui::Button("PLAY", ImVec2(100, 30)))
                paused = false;
        }
        else
        {
            if (ImGui::Button("PAUSE", ImVec2(100, 30)))
                paused = true;
        }

        ImGui::SameLine();
        ImGui::Text(paused ? "(Paused)" : "(Running)");

        ImGui::Separator();

        float& alpha = JuliaService::GetAlphaState();

        ImGui::SliderFloat("Alpha (Time)", &alpha, 0.0f, 6.28318f);

        ImGui::Text("Current C constant:");
        ImGui::Text("Real: %.4f", JuliaService::GetCurrentRc());
        ImGui::Text("Imag: %.4f", JuliaService::GetCurrentRs());

        ImGui::End();
    }

    void EndFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Destroy()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}