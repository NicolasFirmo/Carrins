#pragma once

class RendererAPI
{
public:
	virtual void SetClearColor(const float r, const float g, const float b, const float a) = 0;
	virtual void Clear() = 0;
};