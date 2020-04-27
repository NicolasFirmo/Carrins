#pragma once
#include <GLFW/glfw3.h>

class ImGuiLayer
{
public:
	static void Init(GLFWwindow* window,const char* glsl_version);
	static void Shutdown();

	static void BeginFrame();
	static void EndFrame();

	static void Update(float dt, float& fov);
};
