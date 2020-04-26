#include "OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLCore.h"

std::unique_ptr<VertexArray> VertexArray::Create(std::unique_ptr<VertexBuffer> &&vertexBuffer, std::unique_ptr<IndexBuffer> &&indexBuffer)
{
	return std::make_unique<OpenGLVertexArray>(std::move(vertexBuffer), std::move(indexBuffer));
}

OpenGLVertexArray::OpenGLVertexArray(std::unique_ptr<VertexBuffer> &&vertexBuffer, std::unique_ptr<IndexBuffer> &&indexBuffer) : VertexArray(std::move(vertexBuffer), std::move(indexBuffer))
{
	GLCall(glCreateVertexArrays(1, &m_Id));
	GLCall(glBindVertexArray(m_Id));

	unsigned idx = -1;
	size_t offset = 0;
	for (const auto &attrib : m_VertexBuffer->GetLayout().Attributes)
	{
		GLCall(glEnableVertexAttribArray(++idx));
		GLCall(glVertexAttribPointer(idx, attrib.Size(), GetGLType(attrib.Type), GetGLNormalized(attrib.Type), attrib.Stride(), reinterpret_cast<void *>(offset)));
		offset += attrib.Stride();
	}
}
OpenGLVertexArray::~OpenGLVertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_Id));
}

void OpenGLVertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_Id));
	m_VertexBuffer->Bind();
	m_IndexBuffer->Bind();
}

// Helpers

constexpr int OpenGLVertexArray::GetGLType(VertexLayout::Attribute::T type) const noexcept
{
	using T = VertexLayout::Attribute::T;
	switch (type)
	{
	case T::Float:
		return GL_FLOAT;
	case T::Float2:
		return GL_FLOAT;
	case T::Float3:
		return GL_FLOAT;
	case T::Float4:
		return GL_FLOAT;

	case T::UInt:
		return GL_UNSIGNED_INT;
	case T::UInt2:
		return GL_UNSIGNED_INT;
	case T::UInt3:
		return GL_UNSIGNED_INT;
	case T::UInt4:
		return GL_UNSIGNED_INT;
	}
}

constexpr int OpenGLVertexArray::GetGLNormalized(VertexLayout::Attribute::T type) const noexcept
{
	using T = VertexLayout::Attribute::T;
	switch (type)
	{
	case T::Float:
		return GL_FALSE;
	case T::Float2:
		return GL_FALSE;
	case T::Float3:
		return GL_FALSE;
	case T::Float4:
		return GL_FALSE;

	case T::UInt:
		return GL_TRUE;
	case T::UInt2:
		return GL_TRUE;
	case T::UInt3:
		return GL_TRUE;
	case T::UInt4:
		return GL_TRUE;
	}
}