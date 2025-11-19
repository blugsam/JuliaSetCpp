#pragma once

#include "../JuliaEnums.h"

namespace Backend
{
	bool Init(WindowedMode windowMode);
	void BeginFrame();
	void UpdateSystem();
	void UpdateUtilities();
	void EndFrame();
	void Destroy();
	void SetWindowedMode(const WindowedMode& windowedMode);
	void ToggleFullscreen();
	void ForceCloseWindow();
	bool WindowIsOpen();
	void* GetWindowPointer();
	int GetWindowedWidth();
	int GetWindowedHeight();
	int GetFullScreenWidth();
	int GetFullScreenHeight();
	int GetCurrentWindowWidth();
	int GetCurrentWindowHeight();
	void UpdateLazyKeyPresses();
}