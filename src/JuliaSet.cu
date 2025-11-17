#include <iostream>

#include "Backend/Backend.h"

int main()
{
    if (!Backend::Init(WindowedMode::WINDOWED))
    {
        std::cerr << "ERROR::Backend: Error during backend initialization." << std::endl;
        return -1;
    }

    while (Backend::WindowIsOpen())
    {
        Backend::UpdateUtilities();
        Backend::BeginFrame();
        Backend::EndFrame();
    }

    Backend::Destroy();
}
