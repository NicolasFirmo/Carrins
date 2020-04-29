#pragma once

class IndexBuffer
{
public:
	static std::unique_ptr<IndexBuffer> Create(const unsigned *indices, size_t count);
	IndexBuffer(size_t count) : m_Count(count) {}
	virtual ~IndexBuffer() = default;

	virtual void Bind() const = 0;
	
	size_t GetCount() const { return m_Count; }

protected:
	size_t m_Count;
};