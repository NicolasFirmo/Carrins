#include "App.h"
#include "Core.h"

#include "Inupt.h"

#include "Events/WindowEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

#include "Renderer/Renderer.h"
#include "ImGui/ImGuiLayer.h"

#include <Instrumentation/Profile.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

App App::s_Instance;

static float s_Fov = glm::radians(80.0f);

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

	m_Camera = std::make_unique<PerspectiveCamera>(
		PerspectiveCamera::Position{ 2.0f, 2.0f, 2.0f },
		PerspectiveCamera::Orientation{ -nic::PI / 4, nic::PI / 4 },
		s_Fov,
		float(m_Window->GetWidth()) / float(m_Window->GetHeight()));

	m_Running = true;
}

int App::Run()
{
	NIC_PROFILE_FUNCTION();

	auto& window = *Get().m_Window;
	auto& running = Get().m_Running;

	Renderer::Init();
	ImGuiLayer::Init(static_cast<GLFWwindow*>(window.GetNativeWindow()), glsl_version);

	std::thread cameraCtrlThread([&running]() {
		float dt = 0.0f;
		std::chrono::time_point<std::chrono::high_resolution_clock> tp = std::chrono::high_resolution_clock::now();
		while (running)
		{
			dt = (std::chrono::high_resolution_clock::now() - tp).count() / 1000000000.0f;
			tp = std::chrono::high_resolution_clock::now();
			ControllCamera(dt);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		});

	auto& dt = Get().m_Dt;
	std::chrono::time_point<std::chrono::high_resolution_clock> tp = std::chrono::high_resolution_clock::now();
	while (running)
	{
		NIC_PROFILE_SCOPE("Run Loop");
		dt = (std::chrono::high_resolution_clock::now() - tp).count() / 1000000000.0f;
		tp = std::chrono::high_resolution_clock::now();
		DoFrame(dt, window);
	}
	cameraCtrlThread.join();

	Renderer::Shutdown();
	ImGuiLayer::Shutdown();

	return 0;
}

void App::ShutDown()
{
	NIC_PROFILE_FUNCTION();

	Get().m_Running = false;
}

void App::DoFrame(float dt, Window& window)
{
	NIC_PROFILE_FUNCTION();

	auto& camera = *Get().m_Camera;

	// Input stuff

	camera.SetFOV(s_Fov);
	UpdateImGuiLayerState(Get().m_ImGuiLayerShouldToggle);

	// Rendering stuff

	Renderer::BeginScene(camera);

	for (int i = -3; i < 3; i++)
		for (int j = -3; j < 3; j++)
			for (int k = -3; k < 3; k++)
				Renderer::DrawCube(glm::translate(glm::mat4(1.0f), { 4.0f * i, 4.0f * j, 4.0f * k }) * glm::inverse(glm::eulerAngleYXZ(float(i * nic::PI / 8), float(j * nic::PI / 8), float(k * nic::PI / 8))));

	Renderer::EndScene();

	// ImGui stuff

	ImGuiLayer::BeginFrame();
	bool vSync = window.IsVSync();
	bool fullScreen = window.IsFullScreen();
	ImGuiLayer::Update(dt, s_Fov, vSync, fullScreen);

	ImGuiLayer::EndFrame();

	// Pool events & Swap frame buffer

	window.Update(vSync, fullScreen);
}

void App::ControllCamera(float dt)
{
	auto& camera = *Get().m_Camera;

	if (Input::IsKeyPressed(GLFW_KEY_W))
		camera.TransformPosition(0.0f, 0.0f, -dt);
	else if (Input::IsKeyPressed(GLFW_KEY_S))
		camera.TransformPosition(0.0f, 0.0f, dt);
	if (Input::IsKeyPressed(GLFW_KEY_D))
		camera.TransformPosition(dt, 0.0f, 0.0f);
	else if (Input::IsKeyPressed(GLFW_KEY_A))
		camera.TransformPosition(-dt, 0.0f, 0.0f);
	if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		camera.TransformPosition(0.0f, dt, 0.0f);
	else if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		camera.TransformPosition(0.0f, -dt, 0.0f);

	if (Input::IsKeyPressed(GLFW_KEY_Q))
		camera.TransformOrientation(0.0f, 0.0f, dt);
	else if (Input::IsKeyPressed(GLFW_KEY_E))
		camera.TransformOrientation(0.0f, 0.0f, -dt);
	if (Input::IsKeyPressed(GLFW_KEY_UP))
		camera.TransformOrientation(dt, 0.0f, 0.0f);
	else if (Input::IsKeyPressed(GLFW_KEY_DOWN))
		camera.TransformOrientation(-dt, 0.0f, 0.0f);
	if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
		camera.TransformOrientation(0.0f, -dt, 0.0f);
	else if (Input::IsKeyPressed(GLFW_KEY_LEFT))
		camera.TransformOrientation(0.0f, dt, 0.0f);
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
			Renderer::SetViewport(e.Width, e.Height);
			Get().m_Camera->SetAspectRatio(float(e.Width) / float(e.Height));
#ifdef PLATFORM_WINDOWS
			DoFrame(Get().m_Dt, *Get().m_Window);
#endif
			return true;
		}
		return false;
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
			case GLFW_KEY_F:
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

	if (e.IsHandled())
		return;
}

Window& App::GetWindow() const
{
	return *m_Window;
}