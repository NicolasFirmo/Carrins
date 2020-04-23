#pragma once

class Renderer
{
public:
	static void Init();
	static void BeginScene();
	static void EndScene();

private:
	static std::unique_ptr<class RendererAPI> s_API;
};