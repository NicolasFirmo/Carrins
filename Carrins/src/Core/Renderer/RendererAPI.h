#pragma once
#include "Bindables/VertexArray.h"

class RendererAPI
{
public:
	virtual void SetClearColor(const float r, const float g, const float b, const float a) const = 0;
	virtual void SetViewport(int width, int height) const = 0;
	virtual void Draw(const VertexArray &vertexArray) const = 0;
	virtual void Clear() const = 0;
};