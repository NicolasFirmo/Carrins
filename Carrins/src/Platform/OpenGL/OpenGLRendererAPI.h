#pragma once
#include "Core/Renderer/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
	virtual void SetClearColor(const float r, const float g, const float b, const float a) const override;
	virtual void SetViewport(int width,int height) const override;
	virtual void Draw(const VertexArray &vertexArray) const override;
	virtual void Clear() const override;
};