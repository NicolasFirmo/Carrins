#include "VertexLayout.h"

#include "Instrumentation/Profile.h"

VertexLayout::VertexLayout(const std::initializer_list<Attribute> &attributes) : m_Attributes(attributes)
{
	NIC_PROFILE_FUNCTION();

	for (const auto &attrib : m_Attributes)
		m_Stride += attrib.Size();
}