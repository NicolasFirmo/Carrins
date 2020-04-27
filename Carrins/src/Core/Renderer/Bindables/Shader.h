#pragma once

class Shader
{
public:
	static std::unique_ptr<Shader> Create(const std::string& filepath);
	virtual ~Shader() = default;

	virtual void Bind() const = 0;
};