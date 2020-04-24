#include "App.h"
#include "Inupt.h"

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
	m_Window->SetEventCallback([](Event &e) { Get().OnEvent(e); });
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
		ImGuiLayer::BeginFrame();
		ImGuiLayer::Update();

		Renderer::BeginScene();
		Renderer::EndScene();

		ImGuiLayer::EndFrame();
		window->Update();
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
	if (e.Dispatch<CloseEvent>([](CloseEvent &e) {
				App::ShutDown();
				return true;
			}))
		return;

	if (e.Dispatch<KeyEvent>([](KeyEvent &e) {
				if (e.Code == GLFW_KEY_ESCAPE)
				{
					App::ShutDown();
					DebugLog("Application closed on esc\n");
					return true;
				}
				else
					return false;
			}))
		return;
}

Window &App::GetWindow() const
{
	return *m_Window;
}