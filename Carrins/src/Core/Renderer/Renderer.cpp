#include "Renderer.h"
#include "RendererAPI.h"

#include "Bindables/VertexBuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // Projection Matrices

#include "Instrumentation/Profile.h"

Renderer::Renderer(const std::string& objectName, const std::string& shaderName)
{
	NIC_PROFILE_FUNCTION();

	auto [vertices, indices] = ParseObj("Assets/Objects/" + objectName);

	std::unique_ptr<VertexBuffer> vb = VertexBuffer::Create(vertices.data(), sizeof(Vertex) * vertices.size());
	m_Ib = IndexBuffer::Create(indices.data(), indices.size());

	m_Va = VertexArray::Create(std::move(vb), {
						{VertexLayout::Attribute::T::Float3},
						{VertexLayout::Attribute::T::Float3},
		});

	m_Shader = Shader::Create("Assets/Shaders/" + shaderName);
}

void Renderer::BeginScene(const Camera& camera)
{
	NIC_PROFILE_FUNCTION();

	RendererAPI::Clear();
	m_Shader->Bind();
	m_Va->Bind();
	m_Ib->Bind();

	camera.Bind(*m_Shader);
}

void Renderer::DrawObject(const glm::mat4& transform)
{
	NIC_PROFILE_FUNCTION();

	m_Shader->SetUniformMat4("u_ModelTransform", transform);

	RendererAPI::Draw(m_Ib->GetCount());
}

void Renderer::EndScene()
{
	NIC_PROFILE_FUNCTION();
}

// Helpers

std::pair<std::vector<Renderer::Vertex>, std::vector<unsigned>> Renderer::ParseObj(const std::string& filepath)
{
	NIC_PROFILE_FUNCTION();

	std::ifstream objFile(filepath);
	NIC_ASSERT(objFile.is_open(), "objFile \"" << filepath << "\" could not be opened");
	std::ifstream mtlFile(filepath.substr(0, filepath.size() - 3) + "mtl");
	NIC_ASSERT(objFile.is_open(), "mtlFile \"" << (filepath.substr(0, filepath.size() - 3) + "mtl") << "\" could not be opened");

	std::vector<Vertex::Vec3> positions;
	std::vector<Vertex::Vec3> normals;

	std::unordered_map<std::string, int> verticesMap;

	std::vector<Vertex::RGBA> colors;

	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	std::string line;

	// mtl

	while (std::getline(mtlFile, line))
		if (line.find("newmtl") != std::string::npos)
		{
			do
			{
				std::getline(mtlFile, line);
			} while (line[0] != 'k' && line[1] != 'd');

			std::istringstream iss(line);
			std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
				std::istream_iterator<std::string>());

			colors.push_back({ unsigned char(std::stof(results[1]) * 255),unsigned char(std::stof(results[2]) * 255) ,unsigned char(std::stof(results[3]) * 255),255 });
		}

	// obj

	do
	{
		std::getline(objFile, line);
	} while (line[0] != 'o');


	int idx = 0;
	do
	{
		std::getline(objFile, line);

		// positions

		do
		{
			std::istringstream iss(line);
			std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
				std::istream_iterator<std::string>());

			positions.push_back({ std::stof(results[1]),std::stof(results[2]) ,std::stof(results[3]) });

			std::getline(objFile, line);

		} while (line[0] == 'v' && line[1] == ' ');

		// normals

		do
		{
			std::istringstream iss(line);
			std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
				std::istream_iterator<std::string>());

			normals.push_back({ std::stof(results[1]),std::stof(results[2]) ,std::stof(results[3]) });

			std::getline(objFile, line);

		} while (line[0] == 'v' && line[1] == 'n');

		// faces & indices

		do
		{
			std::getline(objFile, line);
		} while (line[0] != 'f');

		do
		{
			std::istringstream iss(line);
			std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
				std::istream_iterator<std::string>());

			for (size_t i = 0; i < 3; i++)
			{
				if (verticesMap.find(results[i + 1]) == verticesMap.end())
					verticesMap[results[i + 1]] = idx++;

				indices.emplace_back(verticesMap[results[i + 1]]);
			}

			std::getline(objFile, line);
		} while (line[0] == 'f');

	} while (line[0] == 'o');

	// vertices

	vertices.resize(verticesMap.size());

	for (const auto& vertexMap : verticesMap)
	{
		auto posidx = std::stoi(vertexMap.first.substr(0, vertexMap.first.find('/'))) - 1;
		auto normalidx = std::stoi(vertexMap.first.substr(vertexMap.first.rfind('/') + 1)) - 1;

		vertices[vertexMap.second] = {
		positions[posidx],
		normals[normalidx]
		};
	}

	return { vertices,indices };
}
std::pair<std::vector<Renderer::Vertex>, std::vector<unsigned>> Renderer::ParsePly(const std::string& filepath) // Not being used
{
	NIC_PROFILE_FUNCTION();

	std::ifstream objFile(filepath);
	NIC_ASSERT(objFile.is_open(), "objFile \"" << filepath << "\" could not be opened");

	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	std::string line;
	while (std::getline(objFile, line))
	{
		if (line.find("element vertex") != std::string::npos)
		{
			auto VerticeCount = std::stoi(line.substr(line.rfind(' ') + 1));
			vertices.resize(VerticeCount);
		}
		else if (line.find("element face") != std::string::npos)
		{
			auto triangleCount = std::stoi(line.substr(line.rfind(' ') + 1));
			indices.resize(triangleCount * 3);
		}
		else if (line.find("end_header") != std::string::npos)
			break;
	}

	for (auto& v : vertices)
	{
		std::getline(objFile, line);
		std::istringstream iss(line);
		std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
			std::istream_iterator<std::string>());

		v.Position.x = std::stof(results[0]);
		v.Position.y = std::stof(results[1]);
		v.Position.z = std::stof(results[2]);

		v.Normal.x = std::stof(results[3]);
		v.Normal.y = std::stof(results[4]);
		v.Normal.z = std::stof(results[5]);

		//v.Color.r = std::stoi(results[6]);
		//v.Color.g = std::stoi(results[7]);
		//v.Color.b = std::stoi(results[8]);
		//v.Color.a = std::stoi(results[9]);
	}
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		std::getline(objFile, line);
		std::istringstream iss(line);
		std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
			std::istream_iterator<std::string>());

		indices[i + 0] = std::stof(results[1]);
		indices[i + 1] = std::stof(results[2]);
		indices[i + 2] = std::stof(results[3]);
	}

	return { vertices,indices };
}