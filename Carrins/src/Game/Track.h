#pragma once
#include "Core/Renderer/Bindables/Cameras.h"
#include "Core/Renderer/Bindables/Shader.h"
#include "Core/Renderer/Bindables/Texture.h"
#include "Core/Renderer/Bindables/VertexArray.h"
#include "Core/Renderer/Bindables/IndexBuffer.h"

class Track
{
	struct Vertex
	{
		struct Vec2
		{
			float u, v;
		} TexCoord;
	};

public:
	Track(const std::string &name);

	void Draw(const Camera& camera);

private:
	std::unique_ptr<Shader> m_Shader = nullptr;
	std::unique_ptr<Texture> m_HeightMap = nullptr;
	std::unique_ptr<Texture> m_NormalMap = nullptr;
	std::unique_ptr<VertexArray> m_Va = nullptr;
	std::unique_ptr<IndexBuffer> m_Ib = nullptr;
};