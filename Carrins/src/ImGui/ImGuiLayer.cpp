#include "ImGuiLayer.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Instrumentation/Profile.h"

bool ImGuiLayer::s_Enabled = false;
nic::Accumulator<float,30> ImGuiLayer::s_Dts(0.016f);

void ImGuiLayer::BeginFrame()
{
	NIC_PROFILE_FUNCTION();

	if (!s_Enabled)
		return;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
}
void ImGuiLayer::EndFrame()
{
	NIC_PROFILE_FUNCTION();

	if (!s_Enabled)
		return;

	ImGuiIO& io = ImGui::GetIO();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImGuiLayer::OnFrame(float dt, bool& vSync, bool& fullScreen)
{
	NIC_PROFILE_FUNCTION();

	if (!s_Enabled)
		return;

	s_Dts.Push(dt);

	BeginFrame();

	if (ImGui::Begin("App"))
	{
		ImGui::Text("Rodando a %.1ffps (%.3fms/frame)", 1.0f / s_Dts.Average(), s_Dts.Average() * 1000.0f);

		ImGui::Columns(2);
		ImGui::Checkbox("VSync", &vSync);
		ImGui::NextColumn();
		ImGui::Checkbox("Tela cheia", &fullScreen);
	}
	ImGui::End();

	EndFrame();
}

void ImGuiLayer::Init(GLFWwindow* window, const char* glsl_version)
{
	NIC_PROFILE_FUNCTION();

	if (s_Enabled)
		return;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(window, false);
	ImGui_ImplOpenGL3_Init(glsl_version);

	s_Enabled = true;
}
void ImGuiLayer::Shutdown()
{
	NIC_PROFILE_FUNCTION();

	if (!s_Enabled)
		return;

	s_Enabled = false;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
void ImGuiLayer::Toggle(GLFWwindow* window, const char* glsl_version)
{
	if (!s_Enabled)
		Init(window, glsl_version);
	else
		Shutdown();
}