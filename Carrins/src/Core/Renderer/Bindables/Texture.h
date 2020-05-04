#pragma once

class Texture
{
public:
	static std::unique_ptr<Texture> Create(const std::string &filepath);

	virtual void Bind(unsigned slot = 0) const = 0;
};