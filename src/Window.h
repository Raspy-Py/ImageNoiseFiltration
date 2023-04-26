#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <string>
#include <memory>
#include <queue>

class Window
{
public:
	Window(int width, int height, const char* title, bool fullscreen = false);
	~Window();

	GLFWwindow* Get() const;
	bool Open() const;
	float GetAspectRation() const;
	void SwapBuffers();
	void Clear(float r, float g, float b);
	void Close();
	void SetFullScreen(bool flag);
	std::optional<std::string> TryPopDropedFile();

private:
	static void FramebufferSizeCallBack(GLFWwindow* window, int width, int height);
	static void DropCallBack(GLFWwindow* window, int count, const char** paths);

private:
	int m_Width;
	int m_Height;
	bool m_FullScreen;
	std::string m_Title;
	GLFWwindow* m_Window;

	static std::queue<std::string> s_DropedFilesPaths;
};

