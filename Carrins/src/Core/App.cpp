#include "App.h"
#include "Window.h"

#include "Core.h"

#include "ImGui/ImGuiLayer.h"
#include "Renderer/Renderer.h"

App App::s_Instance;

App &App::Get()
{
	return s_Instance;
}

App::App() : m_Window(Window::Create(640, 480, "Carrins"))
{
}

int App::Run()
{
	auto &window = Get().m_Window;

	Renderer::Init();
	ImGuiLayer::Init(reinterpret_cast<GLFWwindow *>(window->GetNativeWindow()), glsl_version);

	while (!window->WindowShouldClose())
	{
		window->PollEvents();
		ImGuiLayer::BeginFrame();
		ImGuiLayer::Update();

		Renderer::BeginScene();
		Renderer::EndScene();

		ImGuiLayer::EndFrame();
		window->SwapBuffers();
	}

	ImGuiLayer::Shutdown();

	return 0;
}