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
	static std::unique_ptr<Texture> Create(std::unique_ptr<Image> image, Filter filter = Filter::None);
	Texture(std::unique_ptr<Image> image) : m_Image(std::move(image)) {}

	virtual void Bind(unsigned slot = 0) const = 0;

	virtual void UpdateTexture(unsigned slot) const = 0;

	Image &GetImage() const { return *m_Image; }

protected:
	std::unique_ptr<Image> m_Image;
};