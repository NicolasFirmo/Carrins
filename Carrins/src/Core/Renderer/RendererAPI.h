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

	virtual void SetClearColor(const float r, const float g, const float b, const float a) const = 0;
	virtual void SetViewport(int width, int height) const = 0;
	virtual void EnableFaceCulling(WindingOrder order) const = 0;
	virtual void EnableDepthTesting() const = 0;
	virtual void Draw(const IndexBuffer &ib) const = 0;
	virtual void Clear() const = 0;
};