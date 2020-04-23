#pragma once
#include "Core/Renderer/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
	virtual void SetClearColor(const float r, const float g, const float b, const float a) override;
	virtual void Clear() override;
};