#pragma once
#include "Core/Renderer/Bindables/VertexArray.h"

class OpenGLVertexArray : public VertexArray
{
public:
	OpenGLVertexArray(std::unique_ptr<VertexBuffer> &&vertexBuffer,const VertexLayout& layout);
	~OpenGLVertexArray();

	virtual void Bind() const override;

private:
	unsigned GetGLType(VertexLayout::Attribute::T type) const noexcept;
	unsigned char GetGLNormalized(VertexLayout::Attribute::T type) const noexcept;

private:
	unsigned m_Id;
};