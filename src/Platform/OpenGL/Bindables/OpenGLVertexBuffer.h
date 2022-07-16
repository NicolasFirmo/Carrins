#pragma once
#include "Core/Renderer/Bindables/VertexBuffer.h"

class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(const void *data, size_t dataSize);
	OpenGLVertexBuffer(size_t dataSize);
	~OpenGLVertexBuffer();

	virtual void Bind() const override;

	virtual void SetData(const void *data, size_t dataSize) const override;

private:
	unsigned m_Id;
};