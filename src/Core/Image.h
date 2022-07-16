#pragma once

class Image
{
public:
	Image(const std::string &filepath);
	Image(const unsigned width, const unsigned height, const unsigned char channels, const unsigned char bitDepth);
	Image(const unsigned width, const unsigned height, const unsigned char channels, const unsigned char bitDepth, const unsigned char *imgBuffer);
	~Image();

	unsigned GetWidth() const { return m_Width; }
	unsigned GetHeight() const { return m_Height; }

	unsigned char GetChannels() const { return m_Channels; }
	unsigned char GetBitDepth() const { return m_BitDepth; }

	unsigned char *GetImgBuffer() const { return m_ImgBuffer; }

private:
	unsigned m_Width, m_Height;
	unsigned char m_Channels, m_BitDepth;

	unsigned char *m_ImgBuffer = nullptr;
};