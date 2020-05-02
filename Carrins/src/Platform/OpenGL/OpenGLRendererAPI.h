#pragma once
#include "Core/Renderer/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
	virtual void SetClearColor_(const float r, const float g, const float b, const float a) const override;
	virtual void SetViewport_(int width, int height) const override;
	virtual void EnableFaceCulling_(WindingOrder order) const override;
	virtual void EnableDepthTesting_() const override;
	virtual void Draw_(const size_t indexCount) const override;
	virtual void Clear_() const override;
};