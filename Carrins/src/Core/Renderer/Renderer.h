#pragma once
#include "Bindables/Cameras.h"

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
	static void Init();
	static void Shutdown();

	static void BeginScene(const Camera& camera);
	static void EndScene();

	static std::pair<std::vector<Vertex>, std::vector<unsigned>> ReadObj(const std::string& filepath);
	static std::pair<std::vector<Vertex>, std::vector<unsigned>> ReadPly(const std::string& filepath);

	static void DrawCube(const glm::mat4& transform);

	static void SetViewport(int width, int height);

private:
	static std::unique_ptr<class RendererAPI> s_API;

	static std::unique_ptr<class Shader> s_Shader;
	static std::unique_ptr<class VertexArray> s_Va;
	static std::unique_ptr<class IndexBuffer> s_Ib;
};