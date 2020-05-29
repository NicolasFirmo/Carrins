#include "App.h"
#include "Core.h"

#include "Events/WindowEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

#include "Renderer/RendererAPI.h"

#include "ImGui/ImGuiLayer.h"
#include "Game/GameLayer.h"

#include <Instrumentation/Profile.h>

#ifndef NDEBUG
struct
{
	float r = 0.0f;
	float g = 0.5f;
	float b = 1.0f;
	float a = 1.0f;
} static constexpr s_ClearColor;
#else
struct
{
	float r = 1.0f;
	float g = 0.5f;
	float b = 0.0f;
	float a = 1.0f;
} static constexpr s_ClearColor;
#endif

App App::s_Instance;

App& App::Get()
{
	return s_Instance;
}

App::~App()
{
	NIC_PROFILE_END_SESSION();
}

App::App() : m_Window(Window::Create(640, 480, "Carrins"))
{
	NIC_PROFILE_BEGIN_SESSION("Carrins", "Profile.json");
	NIC_PROFILE_FUNCTION();

	m_Window->SetEventCallback([](Event& e) { Get().OnEvent(e); });
}

void App::Init()
{
	NIC_PROFILE_FUNCTION();
	auto& window = *Get().m_Window;

	RendererAPI::EnableDepthTesting();
	RendererAPI::EnableFaceCulling(RendererAPI::WindingOrder::CounterClockwise);
	RendererAPI::SetClearColor(s_ClearColor.r, s_ClearColor.g, s_ClearColor.b, s_ClearColor.a);

	ImGuiLayer::Init(static_cast<GLFWwindow*>(window.GetNativeWindow()), glsl_version);
	GameLayer::Init(float(window.GetWidth()) / float(window.GetHeight()));
}
int App::Run()
{
	auto& running = Get().m_Running = true;
	auto& dt = Get().m_Dt;
	std::chrono::time_point<std::chrono::high_resolution_clock> tp = std::chrono::high_resolution_clock::now();

	while (running)
	{
		NIC_PROFILE_SCOPE("Run loop");
		dt = (std::chrono::high_resolution_clock::now() - tp).count() / 1000000000.0f;
		tp = std::chrono::high_resolution_clock::now();

		GameLayer::OnUpdate(dt);
		DoFrame(dt);
	}

	GameLayer::Shutdown();
	ImGuiLayer::Shutdown();

	return 0;
}

void App::ShutDown()
{
	NIC_PROFILE_FUNCTION();

	Get().m_Running = false;
}

void App::DoFrame(float dt)
{
	NIC_PROFILE_FUNCTION();

	auto& window = *Get().m_Window;
	bool vSync = window.IsVSync();
	bool fullScreen = window.IsFullScreen();

	UpdateImGuiLayerState(Get().m_ImGuiLayerShouldToggle);

	RendererAPI::Clear();
	GameLayer::OnRender();

	ImGuiLayer::OnFrame(dt, vSync, fullScreen, GameLayer::s_COR);

	window.Update(vSync, fullScreen);
}

void App::UpdateImGuiLayerState(bool shouldToggle)
{
	if (shouldToggle)
	{
		DebugLog("Toggling ImGuiLayer\n");
		NIC_PROFILE_SCOPE("Toggling ImGuiLayer");

		ImGuiLayer::Toggle(reinterpret_cast<GLFWwindow*>(Get().m_Window->GetNativeWindow()), glsl_version);
		Get().m_ImGuiLayerShouldToggle = false;
	}
}

void App::OnEvent(Event& e)
{
	NIC_PROFILE_FUNCTION();

	e.Dispatch<CloseEvent>([](CloseEvent& e) {
		NIC_PROFILE_SCOPE("App CloseEvent dispatch");
		App::ShutDown();
		return true;
		});
	if (e.IsHandled())
		return;

	e.Dispatch<ResizeEvent>([](ResizeEvent& e) {
		NIC_PROFILE_SCOPE("App ResizeEvent dispatch");
		if (e.Height && e.Width)
		{
			RendererAPI::SetViewport(e.Width, e.Height);
			GameLayer::OnEvent(e);
#ifdef PLATFORM_WINDOWS
			DoFrame(Get().m_Dt);
#endif
		}
		return true;
		});
	e.Dispatch<KeyEvent>([](KeyEvent& e) {
		NIC_PROFILE_SCOPE("App KeyEvent dispatch");
		if (e.Type == KeyEvent::T::Pressed)
		{
			switch (e.Code)
			{
			case GLFW_KEY_ESCAPE:
			{
				App::ShutDown();
				DebugLog("Application closed on esc\n");
				return true;
			}
			case GLFW_KEY_F11:
			{
				Get().GetWindow().ToggleFullScreen();
				return true;
			}
			case GLFW_KEY_F1:
			{
				Get().m_ImGuiLayerShouldToggle = true;
				return true;
			}
			default:
				return false;
			}
		}
		else
			return false;
		});

	if (!e.IsHandled())
		GameLayer::OnEvent(e);
}

Window& App::GetWindow() const
{
	return *m_Window;
}