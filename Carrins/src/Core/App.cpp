#include "App.h"
#include "Window.h"

#include "Core.h"

#include "ImGui/ImGuiLayer.h"
#include "Renderer/Renderer.h"

#include "Events/WindowEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

App App::s_Instance;

App &App::Get()
{
	return s_Instance;
}

App::App() : m_Window(Window::Create(640, 480, "Carrins"))
{
	m_Window->SetEventCallback([](Event &evt) { Get().OnEvent(evt); });
	m_Running = true;
}

int App::Run()
{
	auto &window = Get().m_Window;

	Renderer::Init();
	ImGuiLayer::Init(reinterpret_cast<GLFWwindow *>(window->GetNativeWindow()), glsl_version);

	auto &running = Get().m_Running;
	while (running)
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

void App::ShutDown()
{
	Get().m_Running = false;
}

void App::OnEvent(Event &e)
{
	e.Dispatch<CloseEvent>([](CloseEvent &evt) {
		App::ShutDown();
		return true;
	});

	e.Dispatch<ResizeEvent>([](ResizeEvent &evt) {
		DebugLog("Width: " << evt.Width << " Height: " << evt.Height << '\n');
		return true;
	});
}