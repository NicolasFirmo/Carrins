#pragma once
#include "VertexLayout.h"

class VertexBuffer
{
public:
	static std::unique_ptr<VertexBuffer> Create(const void *data, size_t dataSize, const VertexLayout &layout);
	VertexBuffer(const VertexLayout &layout) : m_Layout(layout) {}

	virtual ~VertexBuffer() = default;
	virtual void Bind() const = 0;

	const VertexLayout &GetLayout() const { return m_Layout; }

protected:
	VertexLayout m_Layout;
};