#pragma once

#include "../Backend/Backend.h"

namespace UserInput
{
	void Init();
	void Update();
	void ClearKeyStates();
	bool KeyPressed(unsigned int keycode);
	bool KeyDown(unsigned int keycode);
}