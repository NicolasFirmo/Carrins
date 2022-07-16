#pragma once
#include "Core/Window.h"

class glfwWindow : public Window
{
public:
	glfwWindow(int width, int height, const char *title);
	~glfwWindow();

	virtual void SetEventCallback(const EventCallbackFn &callback) override;

	virtual void *GetNativeWindow() const override;

	virtual int GetWidth() const override;
	virtual int GetHeight() const override;

	virtual bool IsFullScreen() const override;
	virtual void ToggleFullScreen() override;

	virtual bool IsVSync() const override;
	virtual void ToggleVSync() override;

	virtual void Update(bool vSync, bool fullScreen) override;

private:
	struct Data
	{
		int Width, Height;
		EventCallbackFn EventCallback = nullptr;
	} m_WindowData;

	bool m_VSync = false;
	bool m_FullScreen = false;
	int m_WindowLastWidth = m_WindowData.Width;
	int m_WindowLastHeight = m_WindowData.Height;

	struct GLFWwindow *m_Window;
};