#include "VertexLayout.h"

VertexLayout::VertexLayout(const std::initializer_list<Attribute> &attributes) : m_Attributes(attributes)
{
	for (const auto &attrib : m_Attributes)
		m_Stride += attrib.Size();
}