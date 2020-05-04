#pragma once
#include "Core/Renderer/Bindables/Texture.h"

class OpenGLTexture : public Texture
{
public:
	OpenGLTexture(const int width, const int height, const int channels, const unsigned char *imgBuffer, Filter filter);
	~OpenGLTexture();

	virtual void Bind(unsigned slot) const override;

	virtual void SetImage(const void *imgBuffer, unsigned slot) const override;

	virtual int GetWidth() const override { return m_Width; }
	virtual int GetHeight() const override { return m_Height; }
	virtual int GetChannels() const override { return m_Channels; }
	virtual const unsigned char *GetImgBuffer() const override { return m_ImgBuffer; }

private:
	unsigned m_Id;

	int m_Width, m_Height, m_Channels;
	unsigned char *m_ImgBuffer = nullptr;
};