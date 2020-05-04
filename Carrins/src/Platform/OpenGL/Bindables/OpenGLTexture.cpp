#include "OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLCore.h"

#include "stb/stb_image.h"

#include "Instrumentation/Profile.h"

std::unique_ptr<Texture> Texture::Create(const std::string &filepath)
{
	return std::make_unique<OpenGLTexture>(filepath);
}

OpenGLTexture::OpenGLTexture(const std::string &filepath)
{
	NIC_PROFILE_FUNCTION();

	stbi_set_flip_vertically_on_load(1);
	m_ImgBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_Channels, 0);

	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_Id));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Id));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));

	switch (m_Channels)
	{
	case 1:
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, m_ImgBuffer));
		break;
	case 2:
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, m_Width, m_Height, 0, GL_RG, GL_UNSIGNED_BYTE, m_ImgBuffer));
		break;
	case 3:
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_ImgBuffer));
		break;
	case 4:
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_ImgBuffer));
		break;
	default:
		NIC_ASSERT(false, "Bad number of channels");
	}
}
OpenGLTexture::~OpenGLTexture()
{
	NIC_PROFILE_FUNCTION();

	if (m_ImgBuffer)
		stbi_image_free(m_ImgBuffer);

	GLCall(glDeleteTextures(1, &m_Id));
}

void OpenGLTexture::Bind(unsigned slot) const
{
	NIC_ASSERT(slot < 32, "The max texture slot value in this platform is 31");

	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Id));
}