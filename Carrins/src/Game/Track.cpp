#include "Track.h"

#include "Core/Renderer/Bindables/VertexBuffer.h"
#include "Core/Renderer/RendererAPI.h"

#include "Instrumentation/Profile.h"

Track::Track(const std::string &name)
{
	NIC_PROFILE_FUNCTION();

	m_HeightMap = Texture::Create("Assets/Tracks/" + name + "H.png");

	unsigned width = m_HeightMap->GetWidth();
	unsigned height = m_HeightMap->GetHeight();
	
	unsigned vWidth = width + 1;
	unsigned vHeight = height + 1;

	unsigned pixelCount = width * height;
	unsigned vertexCount = vWidth * vHeight;

	std::vector<Vertex> vertices(vertexCount);
	for (size_t i = 0; i < vWidth; i++)
		for (size_t j = 0; j < vHeight; j++)
		{
			vertices[j * vWidth + i].TexCoord.u = float(i) / (vWidth - 1);
			vertices[j * vWidth + i].TexCoord.v = float(j) / (vHeight - 1);
		}
	auto vb = VertexBuffer::Create(sizeof(Vertex) * vertexCount);
	vb->SetData(vertices.data(), sizeof(Vertex) * vertexCount);
	m_Va = VertexArray::Create(std::move(vb), {{VertexLayout::Attribute::T::Float2}});

	const std::array<unsigned,6> quadIndicesPatern = {
		0,1,vWidth + 1, vWidth + 1,vWidth,0
	};

	std::vector<unsigned> indices(pixelCount * 6);
	for (size_t i = 0, j = 0; i < indices.size(); i += 6, j++)
	{
		if(!((j + 1)%vWidth)) j++;
		std::transform(quadIndicesPatern.begin(), quadIndicesPatern.end(), indices.begin() + i, [&](auto indice) { return indice + j; });
	}
	m_Ib = IndexBuffer::Create(indices.data(), indices.size());

	m_Shader = Shader::Create("Assets/Shaders/TrackShader.glsl");
}

void Track::Draw(const Camera &camera)
{
	NIC_PROFILE_FUNCTION();

	m_Shader->Bind();
	m_HeightMap->Bind(0);
	m_Shader->SetUniformInt("u_HSampler", 0);
	m_Va->Bind();
	m_Ib->Bind();

	camera.Bind(*m_Shader);

	RendererAPI::Draw(m_Ib->GetCount());
}