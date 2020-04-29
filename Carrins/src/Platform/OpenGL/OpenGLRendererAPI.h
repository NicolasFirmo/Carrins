#pragma once
#include "Core/Renderer/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
	virtual void SetClearColor(const float r, const float g, const float b, const float a) const override;
	virtual void SetViewport(int width, int height) const override;
	virtual void EnableFaceCulling(WindingOrder order) const override;
	virtual void EnableDepthTesting() const override;
	virtual void Draw(const size_t count) const override;
	virtual void Clear() const override;
};