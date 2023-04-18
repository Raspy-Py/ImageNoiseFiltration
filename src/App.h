#pragma once
#include "SafeWindows.h"
#include "Exception.h"
#include "Window.h"
#include "Clock.h"
#include "Texture.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <memory>
#include <vector>

class App
{
public:
	App();
	~App();

	void Run();

private:
	void DoFrame(float dt);
	void ProcessEvents(float dt);
	void PollEvents(float dt);

private:
	Clock m_Clock;
	size_t m_FrameCounter;
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Texture> m_Texture;
	std::vector<std::unique_ptr<Texture>> m_PositionTextures;
	std::vector<std::unique_ptr<Texture>> m_VelocityTextures;

	ImGuiIO* m_ImGuiIO;

	const size_t c_TextureSize = 128;
};