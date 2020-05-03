#pragma once
#include <GLFW/glfw3.h>

#include "Utility/Accumulator.hpp"

class ImGuiLayer
{
public:
	static void OnFrame(float dt, bool& vSync, bool& fullScreen);

	static void Init(GLFWwindow* window,const char* glsl_version);
	static void Shutdown();
	static void Toggle(GLFWwindow* window, const char* glsl_version);

private:
	static void BeginFrame();
	static void EndFrame();
private:
	static bool s_Enabled;
	static nic::Accumulator<float,30> s_Dts;
};
