#pragma once

class Texture
{
public:
	static std::unique_ptr<Texture> Create(const std::string &filepath);

	virtual void Bind(unsigned slot = 0) const = 0;

	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual int Getchannels() const = 0;
};