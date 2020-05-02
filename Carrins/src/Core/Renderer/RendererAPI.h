#pragma once
#include "Bindables/IndexBuffer.h"

class RendererAPI
{
public:
	enum class WindingOrder
	{
		Clockwise,
		CounterClockwise,
	};
public:
	static void SetClearColor(const float r, const float g, const float b, const float a) { s_Instance->SetClearColor_(r,g,b,a); }
	static void SetViewport(int width, int height) { s_Instance->SetViewport_(width,height); }
	static void EnableFaceCulling(WindingOrder order) { s_Instance->EnableFaceCulling_(order); }
	static void EnableDepthTesting() { s_Instance->EnableDepthTesting_(); }
	static void Draw(const size_t indexCount) { s_Instance->Draw_(indexCount); }
	static void Clear() { s_Instance->Clear_(); }

protected:
	virtual void SetClearColor_(const float r, const float g, const float b, const float a) const = 0;
	virtual void SetViewport_(int width, int height) const = 0;
	virtual void EnableFaceCulling_(WindingOrder order) const = 0;
	virtual void EnableDepthTesting_() const = 0;
	virtual void Draw_(const size_t indexCount) const = 0;
	virtual void Clear_() const = 0;

protected:
	RendererAPI() = default;
	RendererAPI(const RendererAPI&) = delete;

private:
	static std::unique_ptr<RendererAPI> s_Instance;
};