#include "OpenGLShader.h"
#include "Platform/OpenGL/OpenGLCore.h"

#include <glm/gtc/type_ptr.hpp>

#include "Instrumentation/Profile.h"

std::unique_ptr<Shader> Shader::Create(const std::string &filepath)
{
	NIC_PROFILE_FUNCTION();

	return std::make_unique<OpenGLShader>(filepath);
}

OpenGLShader::OpenGLShader(const std::string &filepath)
{
	NIC_PROFILE_FUNCTION();

	GLCall(m_Id = glCreateProgram());

	auto [vertexSrc, fragmentSrc] = ParseShaders(filepath);
	auto vs = CompileShader(GL_VERTEX_SHADER, vertexSrc.c_str());
	auto fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc.c_str());

	GLCall(glAttachShader(m_Id, vs));
	GLCall(glAttachShader(m_Id, fs));

	GLCall(glLinkProgram(m_Id));
	GLCall(glValidateProgram(m_Id));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
}
OpenGLShader::~OpenGLShader()
{
	NIC_PROFILE_FUNCTION();

	GLCall(glDeleteProgram(m_Id));
}

void OpenGLShader::Bind() const
{
	NIC_PROFILE_FUNCTION();

	GLCall(glUseProgram(m_Id));
}

// Uniforms

void OpenGLShader::SetUniformFloat(const std::string &name, const float value)
{
	GLint location = GetUniformCachedLocation(name);
	GLCall(glUniform1f(location, value));
}
void OpenGLShader::SetUniformFloat2(const std::string &name, const glm::vec2 &value)
{
	GLint location = GetUniformCachedLocation(name);
	GLCall(glUniform2f(location, value[0], value[1]));
}
void OpenGLShader::SetUniformFloat3(const std::string &name, const glm::vec3 &value)
{
	GLint location = GetUniformCachedLocation(name);
	GLCall(glUniform3f(location, value[0], value[1], value[2]));
}
void OpenGLShader::SetUniformFloat4(const std::string &name, const glm::vec4 &value)
{
	GLint location = GetUniformCachedLocation(name);
	GLCall(glUniform4f(location, value[0], value[1], value[2], value[3]));
}

void OpenGLShader::SetUniformMat3(const std::string &name, const glm::mat3 &value)
{
	GLint location = GetUniformCachedLocation(name);
	GLCall(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}
void OpenGLShader::SetUniformMat4(const std::string &name, const glm::mat4 &value)
{
	GLint location = GetUniformCachedLocation(name);
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}

// Helpers

OpenGLShader::Sources OpenGLShader::ParseShaders(const std::string &filepath) const
{
	NIC_PROFILE_FUNCTION();

	std::ifstream shaderFile(filepath);
	NIC_ASSERT(shaderFile.is_open(), "shaderFile \"" << filepath << "\" could not be opened");

	enum
	{
		Unknown = -1,
		Vertex = 0,
		Fragment = 1,
	} type = Unknown;

	std::stringstream sources[sizeof(Sources) / sizeof(Sources::Fragment)];

	std::string line;
	while (std::getline(shaderFile, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = Vertex;
			else if (line.find("fragment") != std::string::npos)
				type = Fragment;
			else
				type = Unknown;
		}
		else
			sources[type] << line << '\n';
	}

	return {sources[Vertex].str(), sources[Fragment].str()};
}
unsigned OpenGLShader::CompileShader(unsigned type, const char *source) const
{
	NIC_PROFILE_FUNCTION();

	GLCall(unsigned id = glCreateShader(type));
	GLCall(glShaderSource(id, 1, &source, nullptr));
	GLCall(glCompileShader(id));

	// validation ------------------------------------
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		NIC_PROFILE_SCOPE("Validation");

		int len;
		char infoLog[4096];
		GLCall(glGetShaderInfoLog(id, 4096, nullptr, infoLog));
		DebugLog((
				type == GL_VERTEX_SHADER ? "Vertex " : type == GL_FRAGMENT_SHADER ? "Fragment " : "Unknown "));
		DebugLog("Shader falied to compile!\n");
		DebugLog(infoLog << '\n');
	}
	// validation ------------------------------------

	return id;
}

int OpenGLShader::GetUniformCachedLocation(const std::string &name)
{
	if (m_UniformLocations.find(name) != m_UniformLocations.end())
	{
		return m_UniformLocations[name];
	}
	else
	{
		NIC_PROFILE_SCOPE("Get uniform location from OpenGL");

		GLCall(int location = m_UniformLocations[name] = glGetUniformLocation(m_Id, name.c_str()));
		return location;
	}
}