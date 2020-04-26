#pragma once
#include "Core/Renderer/Bindables/VertexBuffer.h"

class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(const void *data, size_t dataSize, const VertexLayout &layout);
	~OpenGLVertexBuffer();

	virtual void Bind() const override;

private:
	unsigned m_Id;
};