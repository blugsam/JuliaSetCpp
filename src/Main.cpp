#include <iostream>

#include "UI/TextRenderer.h"
#include "Backend/Backend.h"
#include "JuliaSet/JuliaService.h"
#include "UI/FpsService.h"
#include "Imgui/ImguiService.h"

int main()
{
    std::cout << "when suffering comes, God doesn't leave us to cope with it in our own strength" << std::endl;;

    if (!Backend::Init(WindowedMode::WINDOWED))
    {
        std::cerr << "ERROR::Backend: Error during backend initialization." << std::endl;
        return -1;
    }

    while (Backend::WindowIsOpen())
    {
        Backend::UpdateUtilities();
        Backend::BeginFrame();
        Backend::UpdateServices();

        JuliaService::Render();
        FpsService::Render();
        ImGuiService::Render();
        
        Backend::EndFrame();
    }

    Backend::Destroy();
}
