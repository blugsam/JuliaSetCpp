// K.A.K.A.H.I.
#include <glad/glad.h>

#include <iostream>

#include "UI/TextRenderer.h"
#include "Backend/Backend.h"
#include "JuliaSet/JuliaService.h"
#include "UI/FpsService.h"

int main()
{
    std::cout << "When suffering comes, God doesn't leave us to cope with it in our own strength" << std::endl;;

    if (!Backend::Init(WindowedMode::WINDOWED))
    {
        std::cerr << "ERROR::Backend: Error during backend initialization." << std::endl;
        return -1;
    }

    while (Backend::WindowIsOpen())
    {
        Backend::UpdateUtilities();
        Backend::BeginFrame();
        glClearColor(0.2f, 0.1f, 0.3f, 0.4f);
        glClear(GL_COLOR_BUFFER_BIT);
        Backend::UpdateServices();

        JuliaService::Render();
        FpsService::Render();
        
        Backend::EndFrame();
    }

    Backend::Destroy();
}
