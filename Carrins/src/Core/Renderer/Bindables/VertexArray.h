#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray
{
public:
	static std::unique_ptr<VertexArray> Create(std::unique_ptr<VertexBuffer> &&vertexBuffer, std::unique_ptr<IndexBuffer> &&indexBuffer);
	VertexArray(std::unique_ptr<VertexBuffer> &&vertexBuffer, std::unique_ptr<IndexBuffer> &&indexBuffer) : m_VertexBuffer(std::move(vertexBuffer)), m_IndexBuffer(std::move(indexBuffer)) {}

	virtual ~VertexArray() = default;
	virtual void Bind() const = 0;
	size_t GetIndexCount() const {return m_IndexBuffer->GetCount();};

protected:
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
};