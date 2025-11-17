#include "BackEnd.h"

#include "GLFW.h"
#include "../UserInput/UserInput.h"

namespace Backend
{
	int presentTargetWidth = 0;
	int presentTargetHeight = 0;

	bool Init(WindowedMode windowMode)
	{
		if (!GLFW::Init(windowMode))
		{
			return false;
		}

		UserInput::Init(Backend::GetWindowPointer());

		glfwShowWindow(static_cast<GLFWwindow*>(Backend::GetWindowPointer()));
		return true;
	}

	void BeginFrame()
	{
		GLFW::BeginFrame();
	}

	void EndFrame()
	{
		GLFW::EndFrame();
	}

	void UpdateUtilities()
	{
		UpdateLazyKeyPresses();
		UserInput::Update();
	}

	void Destroy()
	{
		GLFW::Destroy();
	}

	void SetPresentTargetSize(int width, int height)
	{
		presentTargetWidth = width;
		presentTargetHeight = height;
	}
	
	void Backend::ForceCloseWindow()
	{
		GLFW::ForceCloseWindow();
	}

	bool Backend::WindowIsOpen()
	{
		return GLFW::WindowIsOpen();
	}

	void* GetWindowPointer()
	{
		return GLFW::GetWindowPointer();;
	}

	int Backend::GetWindowedWidth()
	{
		return GLFW::GetWindowedWidth();
	}

	int Backend::GetWindowedHeight()
	{
		return GLFW::GetWindowedHeight();
	}

	int Backend::GetFullScreenWidth()
	{
		return GLFW::GetFullScreenWidth();
	}

	int Backend::GetFullScreenHeight()
	{
		return GLFW::GetFullScreenHeight();
	}

	int Backend::GetCurrentWindowWidth()
	{
		return GLFW::GetCurrentWindowWidth();
	}

	int Backend::GetCurrentWindowHeight()
	{
		return GLFW::GetCurrentWindowHeight();
	}

	void UpdateLazyKeyPresses()
	{
		//if (ImGuiBackEnd::HasKeyboardFocus())
		//{
		//	return;
		//}

		if (UserInput::KeyPressed(GLFW_KEY_F11))
		{
			GLFW::ToggleFullscreen();
		}
		if (UserInput::KeyPressed(GLFW_KEY_ESCAPE))
		{
			GLFW::ForceCloseWindow();
		}
		if (UserInput::KeyPressed(GLFW_KEY_F3))
		{
			//Callback::ShowFps();
		}
	}
}