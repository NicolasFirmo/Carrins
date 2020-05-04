#pragma once
#include <glm/glm.hpp>

class Shader
{
public:
	static std::unique_ptr<Shader> Create(const std::string &filepath);
	virtual ~Shader() = default;

	virtual void Bind() const = 0;

	virtual void SetUniformInt(const std::string &name, const int value) = 0;

	virtual void SetUniformFloat(const std::string &name, const float value) = 0;
	virtual void SetUniformFloat2(const std::string &name, const glm::vec2 &value) = 0;
	virtual void SetUniformFloat3(const std::string &name, const glm::vec3 &value) = 0;
	virtual void SetUniformFloat4(const std::string &name, const glm::vec4 &value) = 0;

	virtual void SetUniformMat3(const std::string &name, const glm::mat3 &value) = 0;
	virtual void SetUniformMat4(const std::string &name, const glm::mat4 &value) = 0;
};