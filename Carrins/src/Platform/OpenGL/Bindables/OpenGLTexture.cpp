#include "OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLCore.h"

#include "Instrumentation/Profile.h"

std::unique_ptr<Texture> Texture::Create(std::unique_ptr<Image> image, Filter filter)
{
	return std::make_unique<OpenGLTexture>(std::move(image), filter);
}

OpenGLTexture::OpenGLTexture(std::unique_ptr<Image> image, Filter filter)
		: Texture(std::move(image))
{
	NIC_PROFILE_FUNCTION();

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

	auto width = m_Image->GetWidth();
	auto height = m_Image->GetHeight();
	auto bitDepth = m_Image->GetBitDepth();
	auto imgBuffer = m_Image->GetImgBuffer();

	switch (m_Image->GetChannels())
	{
	case 1:
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (bitDepth == 8 ? GL_R8 : GL_R16), width, height, 0, GL_RED, (bitDepth == 8 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT), imgBuffer));
		break;
	case 2:
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (bitDepth == 8 ? GL_RG8 : GL_RG16), width, height, 0, GL_RG, (bitDepth == 8 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT), imgBuffer));
		break;
	case 3:
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (bitDepth == 8 ? GL_RGB8 : GL_RGB16), width, height, 0, GL_RGB, (bitDepth == 8 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT), imgBuffer));
		break;
	case 4:
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (bitDepth == 8 ? GL_RGBA8 : GL_RGBA16), width, height, 0, GL_RGBA, (bitDepth == 8 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT), imgBuffer));
		break;
	default:
		NIC_ASSERT(false, "Bad number of channels")
	}
}
OpenGLTexture::~OpenGLTexture()
{
	NIC_PROFILE_FUNCTION();

	GLCall(glDeleteTextures(1, &m_Id));
}

void OpenGLTexture::Bind(unsigned slot) const
{
	NIC_ASSERT(slot < 32, "The max texture slot value in this platform is 31")

	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Id));
}

void OpenGLTexture::UpdateTexture(unsigned slot) const
{
	NIC_PROFILE_FUNCTION();

	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Id));

	auto width = m_Image->GetWidth();
	auto height = m_Image->GetHeight();
	auto bitDepth = m_Image->GetBitDepth();
	auto imgBuffer = m_Image->GetImgBuffer();

	switch (m_Image->GetChannels())
	{
	case 1:
		GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, (bitDepth == 8 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT), imgBuffer));
		break;
	case 2:
		GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RG, (bitDepth == 8 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT), imgBuffer));
		break;
	case 3:
		GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, (bitDepth == 8 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT), imgBuffer));
		break;
	case 4:
		GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, (bitDepth == 8 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT), imgBuffer));
		break;
	default:
		NIC_ASSERT(false, "Bad number of channels")
	}
}