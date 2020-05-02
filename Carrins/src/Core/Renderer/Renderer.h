#pragma once
#include "Bindables/Cameras.h"
#include "Bindables/Shader.h"
#include "Bindables/VertexArray.h"
#include "Bindables/IndexBuffer.h"

class Renderer
{
	struct Vertex
	{
		struct Vec3
		{
			float x, y, z;
		} Position, Normal;
		struct RGBA
		{
			unsigned char r, g, b, a;
		};
	};

public:
	Renderer(const std::string& objectName, const std::string& shaderName);

	void BeginScene(const Camera& camera);
	void DrawObject(const glm::mat4& transform);
	void EndScene();

private:
	std::pair<std::vector<Vertex>, std::vector<unsigned>> ParseObj(const std::string& filepath);
	std::pair<std::vector<Vertex>, std::vector<unsigned>> ParsePly(const std::string& filepath);

private:
	std::unique_ptr<Shader> m_Shader = nullptr;
	std::unique_ptr<VertexArray> m_Va = nullptr;
	std::unique_ptr<IndexBuffer> m_Ib = nullptr;
};