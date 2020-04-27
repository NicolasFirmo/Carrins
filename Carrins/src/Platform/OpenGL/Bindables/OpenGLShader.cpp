#include "OpenGLShader.h"
#include "Platform/OpenGL/OpenGLCore.h"

std::unique_ptr<Shader> Shader::Create(const std::string &filepath)
{
	return std::make_unique<OpenGLShader>(filepath);
}

OpenGLShader::OpenGLShader(const std::string &filepath)
{
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
	GLCall(glDeleteProgram(m_Id));
}

void OpenGLShader::Bind() const
{
	GLCall(glUseProgram(m_Id));
}

// Helpers

OpenGLShader::Sources OpenGLShader::ParseShaders(const std::string &filepath) const
{
	std::ifstream file(filepath);

	enum
	{
		Unknown = -1,
		Vertex = 0,
		Fragment = 1,
	} type = Unknown;

	std::stringstream sources[sizeof(Sources) / sizeof(Sources::Fragment)];

	std::string line;
	while (std::getline(file, line))
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
	GLCall(unsigned id = glCreateShader(type));
	GLCall(glShaderSource(id, 1, &source, nullptr));
	GLCall(glCompileShader(id));

	// validation ------------------------------------
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
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