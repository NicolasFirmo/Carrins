#pragma once

class Window
{
public:
	static std::unique_ptr<Window> Create(int width, int height, const char* title);
	virtual ~Window() = default;

	virtual void* GetNativeWindow() = 0;

	virtual void EnableVSync() = 0;
	virtual void DisableVSync() = 0;

	virtual void PollEvents() = 0;
	virtual void SwapBuffers() = 0;

	virtual bool WindowShouldClose() = 0;
};