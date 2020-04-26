#pragma once
#include "Core/Renderer/Bindables/VertexArray.h"

class OpenGLVertexArray : public VertexArray
{
public:
	OpenGLVertexArray(std::unique_ptr<VertexBuffer> &&vertexBuffer, std::unique_ptr<IndexBuffer> &&indexBuffer);
	~OpenGLVertexArray();

	virtual void Bind() const override;

private:
	constexpr int GetGLType(VertexLayout::Attribute::T type) const noexcept;
	constexpr int GetGLNormalized(VertexLayout::Attribute::T type) const noexcept;

private:
	unsigned m_Id;
};