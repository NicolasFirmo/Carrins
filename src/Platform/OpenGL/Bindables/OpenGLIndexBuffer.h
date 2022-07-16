#pragma once
#include "Core/Renderer/Bindables/IndexBuffer.h"

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(const unsigned* indices, size_t count);
	~OpenGLIndexBuffer();

	virtual void Bind() const override;

private:
	unsigned m_Id;
};