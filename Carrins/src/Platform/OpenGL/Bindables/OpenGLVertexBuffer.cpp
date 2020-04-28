#include "OpenGLVertexBuffer.h"
#include "Platform/OpenGL/OpenGLCore.h"

#include "Instrumentation/Profile.h"

std::unique_ptr<VertexBuffer> VertexBuffer::Create(const void *data, size_t dataSize, const VertexLayout &layout)
{
	NIC_PROFILE_FUNCTION();

	return std::make_unique<OpenGLVertexBuffer>(data, dataSize, layout);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const void *data, size_t dataSize, const VertexLayout &layout) : VertexBuffer(layout)
{
	NIC_PROFILE_FUNCTION();

	GLCall(glGenBuffers(1, &m_Id));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
	GLCall(glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW));
}
OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	NIC_PROFILE_FUNCTION();

	GLCall(glDeleteBuffers(1, &m_Id));
}

void OpenGLVertexBuffer::Bind() const
{
	NIC_PROFILE_FUNCTION();

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
}