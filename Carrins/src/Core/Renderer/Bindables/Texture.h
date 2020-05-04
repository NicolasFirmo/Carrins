#pragma once

class Texture
{
public:
	enum class Filter
	{
		None = 0,
		Bilinear,
	};
public:
	static std::unique_ptr<Texture> Create(const std::string &filepath, Filter filter = Filter::None);
	static std::unique_ptr<Texture> Create(const int width, const int height, const int channels, const unsigned char *imgBuffer, Filter filter = Filter::None);

	virtual void Bind(unsigned slot = 0) const = 0;

	virtual void SetImage(const void *imgBuffer, unsigned slot) const = 0;

	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual int GetChannels() const = 0;
	virtual const unsigned char *GetImgBuffer() const = 0;
};