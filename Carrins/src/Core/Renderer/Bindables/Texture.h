#pragma once
#include "Core/Image.h"

class Texture
{
public:
	enum class Filter
	{
		None = 0,
		Bilinear,
	};

public:
	static std::unique_ptr<Texture> Create(const Image& image, Filter filter = Filter::None);

	virtual void Bind(unsigned slot = 0) const = 0;

	virtual void SetImage(const void *imgBuffer, unsigned slot) const = 0;

	virtual unsigned GetWidth() const = 0;
	virtual unsigned GetHeight() const = 0;
	virtual unsigned char GetChannels() const = 0;
	virtual const unsigned char *GetImgBuffer() const = 0;
};