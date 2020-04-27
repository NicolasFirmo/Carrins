#pragma once

class Renderer
{
public:
	static void Init();
	static void Shutdown();

	static void BeginScene();
	static void EndScene();

	static void Draw(const class VertexArray& va);
	static void SetViewport(int width, int height);

private:
	static std::unique_ptr<class RendererAPI> s_API;
};