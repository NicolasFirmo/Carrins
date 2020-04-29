#pragma once

class VertexBuffer
{
public:
	static std::unique_ptr<VertexBuffer> Create(const void *data, size_t dataSize);
	static std::unique_ptr<VertexBuffer> Create(size_t dataSize);

	virtual void Bind() const = 0;

	virtual void SetData(const void *data, size_t dataSize) const = 0;
};