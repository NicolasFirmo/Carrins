#pragma once
#include <GLFW/glfw3.h>

class ImGuiLayer
{
public:
	static void Init(GLFWwindow* window,const char* glsl_version);
	static void Shutdown();
	static void Toggle(GLFWwindow* window, const char* glsl_version);

	static void BeginFrame();
	static void EndFrame();

	static void Update(float dt, float& fov, bool& vSync, bool& fullScreen);
private:
	static bool s_Enabled;
};
