#include "ViewportService.h"
#include "../Backend/Backend.h" 
#include <glm/glm.hpp>

namespace ViewportService
{
    Viewport viewport;

    void Init()
    {
        viewport.SetPosition(glm::vec2(0.0f, 0.0f));
        viewport.SetSize(glm::vec2(1.0f, 1.0f));

        viewport.SetPerspective(1.0f, 0.1f, 1000.0f);

        Update();
    }

    void Update()
    {
        viewport.Update();
    }

    Viewport* GetMainViewport()
    {
        return &viewport;
    }
}