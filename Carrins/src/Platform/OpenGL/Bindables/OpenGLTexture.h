#pragma once
#include "Core/Renderer/Bindables/Texture.h"

class OpenGLTexture : public Texture
{
public:
	OpenGLTexture(const std::string &filepath);
	~OpenGLTexture();

	virtual void Bind(unsigned slot ) const override;

private:
	unsigned m_Id;

	int m_Width, m_Height, m_Channels;
	unsigned char* m_ImgBuffer = nullptr;
};