#pragma once

class Renderer
{
public:
	static void Init();
	static void BeginScene();
	static void Draw(const class VertexArray& va);
	static void EndScene();
	static void Shutdown();

private:
	static std::unique_ptr<class RendererAPI> s_API;
};