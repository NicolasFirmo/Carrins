#pragma once
#include "Events/Event.h"

class Window
{
public:
	static std::unique_ptr<Window> Create(int width, int height, const char *title);
	virtual ~Window() = default;

	using EventCallbackFn = void(*)(Event &);
	virtual void SetEventCallback(const EventCallbackFn &callback) = 0;

	virtual void *GetNativeWindow() const = 0;

	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;

	virtual bool IsVSync() const = 0;
	virtual void EnableVSync() = 0;
	virtual void DisableVSync() = 0;

	virtual void Update() const = 0;
};