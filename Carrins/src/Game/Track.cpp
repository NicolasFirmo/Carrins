#include "Track.h"

#include "Core/Renderer/Bindables/VertexBuffer.h"
#include "Core/Renderer/RendererAPI.h"

#include "Utility/BumpToNormalMap.h"

#include "Instrumentation/Profile.h"

Track::Track(const std::string &name)
{
	NIC_PROFILE_FUNCTION();

	m_HeightMap = Texture::Create(std::make_unique<Image>("Assets/Tracks/" + name + "H.png"));
	auto width = m_HeightMap->GetImage().GetWidth();
	auto height = m_HeightMap->GetImage().GetHeight();

	InitBumpToNormalMap(width, height);

	m_NormalMap = Texture::Create(std::make_unique<Image>(width, height, 4, 8), Texture::Filter::Bilinear);

	auto pixelCount = width * height;

	std::vector<Vertex> vertices(pixelCount);
	for (size_t i = 0; i < width; i++)
		for (size_t j = 0; j < height; j++)
		{
			vertices[j * width + i].TexCoord.u = float(i) / (width - 1);
			vertices[j * width + i].TexCoord.v = float(j) / (height - 1);
		}
	auto vb = VertexBuffer::Create(sizeof(Vertex) * pixelCount);
	vb->SetData(vertices.data(), sizeof(Vertex) * pixelCount);
	m_Va = VertexArray::Create(std::move(vb), {{VertexLayout::Attribute::T::Float2}});

	const std::array<unsigned, 6> quadIndicesPatern = {
			0, 1, width + 1, width + 1, width, 0};

	auto quadCount = (width - 1) * (height - 1);

	std::vector<unsigned> indices(quadCount * 6);
	for (unsigned i = 0, j = 0; i < indices.size(); i += 6, j++)
	{
		if (!((j + 1) % width))
			j++;
		std::transform(quadIndicesPatern.begin(), quadIndicesPatern.end(), indices.begin() + i, [&](auto indice) { return indice + j; });
	}
	m_Ib = IndexBuffer::Create(indices.data(), indices.size());

	m_Shader = Shader::Create("Assets/Shaders/TrackShader.glsl");
}

Track::~Track()
{
	ShutdownBumpToNormalMap();
}

void Track::Draw(const Camera &camera)
{
	NIC_PROFILE_FUNCTION();

	{
		NIC_PROFILE_SCOPE("Cuda Kernel");
		KernelBumpToNormalMap((const unsigned short *)m_HeightMap->GetImage().GetImgBuffer(), m_NormalMap->GetImage().GetImgBuffer());
	}

	m_Shader->Bind();
	m_HeightMap->Bind(0);
	m_Shader->SetUniformInt("u_HSampler", 0);
	m_NormalMap->Bind(1);
	m_Shader->SetUniformInt("u_NSampler", 1);
	m_NormalMap->UpdateTexture(1);
	m_Va->Bind();
	m_Ib->Bind();

	camera.Bind(*m_Shader);

	RendererAPI::Draw(m_Ib->GetCount());
}