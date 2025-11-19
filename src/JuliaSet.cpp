#include <iostream>

// i'm gonna kill myself
#include <glad/glad.h>
#include "UI/TextRenderer.h"
#include "UI/FpsService.h"
#include "Backend/Backend.h"

int main()
{
    std::cout << "When suffering comes, God doesn't leave us to cope with it in our own strength" << "\n";

    if (!Backend::Init(WindowedMode::WINDOWED))
    {
        std::cerr << "ERROR::Backend: Error during backend initialization." << std::endl;
        return -1;
    }

    while (Backend::WindowIsOpen())
    {
        Backend::UpdateUtilities();
        Backend::BeginFrame();
        Backend::UpdateSystem();

        glClearColor(0.2f, 0.1f, 0.3f, 0.4f);
        glClear(GL_COLOR_BUFFER_BIT);

        FpsService::UpdateFps();
        TextRenderer::Render("Hello!", 300.0f, 400.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
        Backend::EndFrame();
    }

    Backend::Destroy();
}
