#include "OpenGLIndexBuffer.h"
#include "Platform/OpenGL/OpenGLCore.h"

#include "Instrumentation/Profile.h"

std::unique_ptr<IndexBuffer> IndexBuffer::Create(const unsigned *indices, size_t count)
{
	NIC_PROFILE_FUNCTION();

	return std::make_unique<OpenGLIndexBuffer>(indices, count);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(const unsigned *indices, size_t count) : IndexBuffer(count)
{
	NIC_PROFILE_FUNCTION();

	GLCall(glGenBuffers(1, &m_Id));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned), indices, GL_STATIC_DRAW));
}
OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	NIC_PROFILE_FUNCTION();

	GLCall(glDeleteBuffers(1, &m_Id));
}

void OpenGLIndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id));
}