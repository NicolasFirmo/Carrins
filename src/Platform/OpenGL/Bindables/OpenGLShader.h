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

	virtual void SetUniformInt(const std::string &name, const int value) override;

	virtual void SetUniformFloat(const std::string &name, const float value) override;
	virtual void SetUniformFloat2(const std::string &name, const glm::vec2 &value) override;
	virtual void SetUniformFloat3(const std::string &name, const glm::vec3 &value) override;
	virtual void SetUniformFloat4(const std::string &name, const glm::vec4 &value) override;

	virtual void SetUniformMat3(const std::string &name, const glm::mat3 &value) override;
	virtual void SetUniformMat4(const std::string &name, const glm::mat4 &value) override;

private:
	Sources ParseShaders(const std::string &filepath) const;
	unsigned CompileShader(unsigned type, const char *source) const;

	int GetUniformCachedLocation(const std::string &name);

private:
	unsigned m_Id;

	std::unordered_map<std::string, int> m_UniformLocations;
};