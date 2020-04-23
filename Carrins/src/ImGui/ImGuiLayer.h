#pragma once

class ImGuiLayer
{
public:
	static void Init(struct GLFWwindow* window,const char* glsl_version);
	static void Shutdown();

	static void BeginFrame();
	static void EndFrame();

	static void Update();
};
