#pragma once
#include "Core/Renderer/Bindables/Texture.h"

class OpenGLTexture : public Texture
{
public:
	OpenGLTexture(std::unique_ptr<Image> image, Filter filter);
	~OpenGLTexture();

	virtual void Bind(unsigned slot) const override;

	virtual void UpdateTexture(unsigned slot) const override;

private:
	unsigned m_Id;
};