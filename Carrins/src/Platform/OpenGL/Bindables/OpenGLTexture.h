#pragma once
#include "Core/Renderer/Bindables/Texture.h"

class OpenGLTexture : public Texture
{
public:
	OpenGLTexture(const Image& image, Filter filter);
	~OpenGLTexture();

	virtual void Bind(unsigned slot) const override;

	virtual void SetImage(const void *imgBuffer, unsigned slot) const override;

	virtual unsigned GetWidth() const override { return m_Width; }
	virtual unsigned GetHeight() const override { return m_Height; }
	virtual unsigned char GetChannels() const override { return m_Channels; }
	virtual const unsigned char *GetImgBuffer() const override { return m_ImgBuffer; }

private:
	unsigned m_Id;

	unsigned m_Width, m_Height;
	unsigned char m_Channels;
	unsigned char *m_ImgBuffer = nullptr;
};