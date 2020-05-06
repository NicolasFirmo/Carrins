#pragma once
#include "Core.h"

class VertexLayout
{
public:
	struct Attribute
	{
		enum class T
		{
			Float,
			Float2,
			Float3,
			Float4,

			UInt,
			UInt2,
			UInt3,
			UInt4,

			UChar,
			UChar2,
			UChar3,
			UChar4,
		} Type;

		size_t Count() const
		{
			switch (Type)
			{
			case T::Float:
				return 1;
			case T::Float2:
				return 2;
			case T::Float3:
				return 3;
			case T::Float4:
				return 4;

			case T::UInt:
				return 1;
			case T::UInt2:
				return 2;
			case T::UInt3:
				return 3;
			case T::UInt4:
				return 4;

			case T::UChar:
				return 1;
			case T::UChar2:
				return 2;
			case T::UChar3:
				return 3;
			case T::UChar4:
				return 4;

			default:
				NIC_ASSERT(false, "Bad vertex attribute type")
				return 0;
			}
		};
		size_t Size() const
		{
			switch (Type)
			{
			case T::Float:
				return 1 * sizeof(float);
			case T::Float2:
				return 2 * sizeof(float);
			case T::Float3:
				return 3 * sizeof(float);
			case T::Float4:
				return 4 * sizeof(float);

			case T::UInt:
				return 1 * sizeof(unsigned);
			case T::UInt2:
				return 2 * sizeof(unsigned);
			case T::UInt3:
				return 3 * sizeof(unsigned);
			case T::UInt4:
				return 4 * sizeof(unsigned);

			case T::UChar:
				return 1 * sizeof(unsigned char);
			case T::UChar2:
				return 2 * sizeof(unsigned char);
			case T::UChar3:
				return 3 * sizeof(unsigned char);
			case T::UChar4:
				return 4 * sizeof(unsigned char);

			default:
				NIC_ASSERT(false, "Bad vertex attribute type")
				return 0;
			}
		};
	};
	VertexLayout(const std::initializer_list<Attribute> &attributes);
	const std::vector<Attribute>& GetAttributes() const { return m_Attributes; }
	int Stride() const { return m_Stride; }

private:
	std::vector<Attribute> m_Attributes;
	int m_Stride = 0;
};