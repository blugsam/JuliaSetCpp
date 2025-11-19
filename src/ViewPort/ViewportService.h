#pragma once
#include "Viewport.h"
#include <vector>

namespace ViewportService
{
    void Init();
    void Update();
    Viewport* GetMainViewport();
}