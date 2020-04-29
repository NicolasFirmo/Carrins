#pragma once
#include "VertexBuffer.h"
#include "VertexLayout.h"

class VertexArray
{
public:
	static std::unique_ptr<VertexArray> Create(std::unique_ptr<VertexBuffer> &&vertexBuffer, const VertexLayout &layout);
	VertexArray(std::unique_ptr<VertexBuffer> &&vertexBuffer, const VertexLayout &layout) : m_VertexBuffer(std::move(vertexBuffer)), m_Layout(layout) {}
	virtual ~VertexArray() = default;

	virtual void Bind() const = 0;

protected:
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	VertexLayout m_Layout;
};