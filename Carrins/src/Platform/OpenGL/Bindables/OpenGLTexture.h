#pragma once
#include "Core/Renderer/Bindables/Texture.h"

class OpenGLTexture : public Texture
{
public:
	OpenGLTexture(const std::string &filepath);
	~OpenGLTexture();

	virtual void Bind(unsigned slot) const override;

	virtual int GetWidth() const override { return m_Width; }
	virtual int GetHeight() const override { return m_Height; }
	virtual int Getchannels() const override { return m_Channels; }

private:
	unsigned m_Id;

	int m_Width, m_Height, m_Channels;
	unsigned char *m_ImgBuffer = nullptr;
};