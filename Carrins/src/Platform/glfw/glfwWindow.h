#pragma once
#include "Core/Window.h"

class glfwWindow : public Window
{
public:
	glfwWindow(int width, int height, const char *title);
	~glfwWindow();

	virtual void SetEventCallback(const EventCallbackFn &callback) override;

	virtual void *GetNativeWindow() override;

	virtual int GetWidth() const override;
	virtual int GetHeight() const override;

	virtual bool IsVSync() const override;
	virtual void EnableVSync() override;
	virtual void DisableVSync() override;

	virtual void PollEvents() override;
	virtual void SwapBuffers() override;

private:
	struct Data
	{
		int Width, Height;
		bool VSync = false;
		EventCallbackFn EventCallback = nullptr;
	} m_Data;

	struct GLFWwindow *m_Window;
};