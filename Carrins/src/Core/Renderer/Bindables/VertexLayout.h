#pragma once

struct VertexLayout
{
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
		} Type;
		std::string Name;

		size_t Size() const {
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
			}
		};
		size_t Stride() const {
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
				return 1 * sizeof(unsigned int);
			case T::UInt2:
				return 2 * sizeof(unsigned int);
			case T::UInt3:
				return 3 * sizeof(unsigned int);
			case T::UInt4:
				return 4 * sizeof(unsigned int);
			}
		};
	};
	std::vector<Attribute> Attributes;
	VertexLayout(const std::initializer_list<Attribute>& attributes) : Attributes(attributes){}
};