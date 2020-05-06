#include "OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLCore.h"

#include "Instrumentation/Profile.h"

std::unique_ptr<VertexArray> VertexArray::Create(std::unique_ptr<VertexBuffer> &&vertexBuffer,const VertexLayout& layout)
{
	NIC_PROFILE_FUNCTION();

	return std::make_unique<OpenGLVertexArray>(std::move(vertexBuffer), layout);
}

OpenGLVertexArray::OpenGLVertexArray(std::unique_ptr<VertexBuffer> &&vertexBuffer, const VertexLayout& layout) : VertexArray(std::move(vertexBuffer), layout)
{
	NIC_PROFILE_FUNCTION();

	GLCall(glCreateVertexArrays(1, &m_Id));
	GLCall(glBindVertexArray(m_Id));

	m_VertexBuffer->Bind();

	unsigned idx = -1;
	size_t offset = 0;
	for (const auto &attrib : layout.GetAttributes())
	{
		NIC_PROFILE_SCOPE("Setting Attribute");

		GLCall(glEnableVertexAttribArray(++idx));
		GLCall(glVertexAttribPointer(idx, attrib.Count(), GetGLType(attrib.Type), GetGLNormalized(attrib.Type), layout.Stride(), reinterpret_cast<void *>(offset)));
		offset += attrib.Size();
	}
}
OpenGLVertexArray::~OpenGLVertexArray()
{
	NIC_PROFILE_FUNCTION();

	GLCall(glDeleteVertexArrays(1, &m_Id));
}

void OpenGLVertexArray::Bind() const
{
	NIC_PROFILE_FUNCTION();

	GLCall(glBindVertexArray(m_Id));
}

// Helpers

unsigned OpenGLVertexArray::GetGLType(VertexLayout::Attribute::T type) const noexcept
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

	case T::UChar:
		return GL_UNSIGNED_BYTE;
	case T::UChar2:
		return GL_UNSIGNED_BYTE;
	case T::UChar3:
		return GL_UNSIGNED_BYTE;
	case T::UChar4:
		return GL_UNSIGNED_BYTE;

	default:
		NIC_ASSERT(false, "Bad vertex attribute type")
		return 0;
	}
}
unsigned char OpenGLVertexArray::GetGLNormalized(VertexLayout::Attribute::T type) const noexcept
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

	case T::UChar:
		return GL_TRUE;
	case T::UChar2:
		return GL_TRUE;
	case T::UChar3:
		return GL_TRUE;
	case T::UChar4:
		return GL_TRUE;

	default:
		NIC_ASSERT(false, "Bad vertex attribute type")
		return 0;
	}
}