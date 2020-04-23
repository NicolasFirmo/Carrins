#pragma once
#include "Core/Window.h"

class glfwWindow : public Window
{
public:
	glfwWindow(int width, int height, const char *title);
	~glfwWindow();

	virtual void* GetNativeWindow() override;

	virtual void EnableVSync() override;
	virtual void DisableVSync() override;

	virtual void PollEvents() override;
	virtual void SwapBuffers() override;

	virtual bool WindowShouldClose() override;
private:
	struct GLFWwindow *m_Window;
};