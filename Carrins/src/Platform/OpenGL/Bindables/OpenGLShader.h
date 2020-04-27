#pragma once
#include "Core/Renderer/Bindables/Shader.h"

class OpenGLShader : public Shader
{
	struct Sources
	{
		std::string Vertex;
		std::string Fragment;
	};

public:
	OpenGLShader(const std::string &filepath);
	~OpenGLShader();

	virtual void Bind() const override;

private:
	Sources ParseShaders(const std::string &filepath) const;
	unsigned CompileShader(unsigned type, const char *source) const;

private:
	unsigned m_Id;
};