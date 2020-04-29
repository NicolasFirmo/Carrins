#pragma once
#include "Bindables/Cameras.h"

class Renderer
{
public:
	static void Init();
	static void Shutdown();

	static void BeginScene(const Camera& camera);
	static void EndScene();

	static void StageCube(float x, float y, float z);

	static void SetViewport(int width, int height);

private:
	static void Flush();

private:
	static std::unique_ptr<class RendererAPI> s_API;

	static std::unique_ptr<class Shader> s_Shader;
	static std::unique_ptr<class VertexArray> s_Va;
	static std::unique_ptr<class IndexBuffer> s_Ib;

	static size_t s_StagedCubesCount;
};