#pragma once

namespace JuliaService
{
    void Init();
    void BeginFrame();
    void Render();
    void Destroy();

    bool& GetPausedState();
    float& GetAlphaState();

    float GetCurrentRc();
    float GetCurrentRs();
}