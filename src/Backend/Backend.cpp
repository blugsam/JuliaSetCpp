#include "Backend.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../API/GL_Backend.h"
#include "GLFW.h"
#include "../UserInput/UserInput.h"
#include "../UI/TextRenderer.h"
#include "../ViewPort/ViewPortService.h"
#include "../UI/FpsService.h"
#include "../JuliaSet/JuliaService.h"
#include "../Imgui/ImguiService.h"

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

		OpenGLBackend::Init();
		UserInput::Init();
		JuliaService::Init();
		TextRenderer::Init();
		ImGuiService::Init();

		glfwShowWindow(static_cast<GLFWwindow*>(Backend::GetWindowPointer()));

		ViewportService::Init();

		return true;
	}

	void BeginFrame()
	{
		GLFW::BeginFrame();
		JuliaService::BeginFrame();
		ImGuiService::BeginFrame();
		TextRenderer::BeginFrame();
		OpenGLBackend::BeginFrame();
	}

	void UpdateServices()
	{
		ViewportService::Update();
		FpsService::Update();
	}

	void UpdateUtilities()
	{
		UpdateLazyKeyPresses();
		UserInput::Update();
	}

	void EndFrame()
	{
		ImGuiService::EndFrame();
		GLFW::EndFrame();
	}

	void Destroy()
	{
		TextRenderer::Destroy();
		JuliaService::Destroy();
		ImGuiService::Destroy();
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

	double Backend::GetTime()
	{
		return GLFW::GetTime();
	}
}