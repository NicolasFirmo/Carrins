#include "OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLCore.h"

#include "Instrumentation/Profile.h"

std::unique_ptr<Texture> Texture::Create(const Image& image, Filter filter)
{
	return std::make_unique<OpenGLTexture>(image, filter);
}

OpenGLTexture::OpenGLTexture(const Image& image, Filter filter)
		: m_Width(image.GetWidth()), m_Height(image.GetHeight()), m_Channels(image.GetChannels())
{
	NIC_PROFILE_FUNCTION();

	m_ImgBuffer = new unsigned char[image.GetWidth() * image.GetBitDepth()/8 * image.GetChannels() * image.GetHeight()];
	std::memcpy(m_ImgBuffer, image.GetImgBuffer(), image.GetWidth() * image.GetBitDepth()/8 * image.GetChannels() * image.GetHeight());

	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_Id));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Id));

	switch (filter)
	{
	case Filter::None:
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		break;
	case Filter::Bilinear:
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		break;
	default:
		NIC_ASSERT(false, "Bad filter")
	}

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));

	switch (m_Channels)
	{
	case 1:
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (image.GetBitDepth() == 8 ? GL_R8 : GL_R16), m_Width, m_Height, 0, GL_RED, (image.GetBitDepth() == 8 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT), m_ImgBuffer));
		break;
	case 2:
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (image.GetBitDepth() == 8 ? GL_RG8 : GL_RG16), m_Width, m_Height, 0, GL_RG, (image.GetBitDepth() == 8 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT), m_ImgBuffer));
		break;
	case 3:
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (image.GetBitDepth() == 8 ? GL_RGB8 : GL_RGB16), m_Width, m_Height, 0, GL_RGB, (image.GetBitDepth() == 8 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT), m_ImgBuffer));
		break;
	case 4:
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (image.GetBitDepth() == 8 ? GL_RGBA8 : GL_RGBA16), m_Width, m_Height, 0, GL_RGBA, (image.GetBitDepth() == 8 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT), m_ImgBuffer));
		break;
	default:
		NIC_ASSERT(false, "Bad number of channels")
	}
}
OpenGLTexture::~OpenGLTexture()
{
	NIC_PROFILE_FUNCTION();

	if (m_ImgBuffer)
		delete[] m_ImgBuffer;

	GLCall(glDeleteTextures(1, &m_Id));
}

void OpenGLTexture::Bind(unsigned slot) const
{
	NIC_ASSERT(slot < 32, "The max texture slot value in this platform is 31")

	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Id));
}

void OpenGLTexture::SetImage(const void *imgBuffer, unsigned slot) const
{
	NIC_PROFILE_FUNCTION();

	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Id));
	switch (m_Channels)
	{
	case 1:
		GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RED, GL_UNSIGNED_BYTE, imgBuffer));
		break;
	case 2:
		GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RG, GL_UNSIGNED_BYTE, imgBuffer));
		break;
	case 3:
		GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, imgBuffer));
		break;
	case 4:
		GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, imgBuffer));
		break;
	default:
		NIC_ASSERT(false, "Bad number of channels")
	}
}