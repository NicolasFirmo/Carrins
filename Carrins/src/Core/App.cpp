#include "App.h"
#include "Inupt.h"

#include "Core.h"

#include "ImGui/ImGuiLayer.h"
#include "Renderer/Renderer.h"

#include "Events/WindowEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

#include "Renderer/Bindables/VertexBuffer.h"
#include "Renderer/Bindables/IndexBuffer.h"
#include "Renderer/Bindables/VertexArray.h"
#include "Renderer/Bindables/Shader.h"
#include "Renderer/Bindables/Cameras.h"

#include <glm/gtc/matrix_transform.hpp> // Projection Matrices

#include <Instrumentation/Profile.h>

App App::s_Instance;

static float s_Fov = glm::radians(80.0f);

App &App::Get()
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

	m_Window->SetEventCallback([](Event &e) { Get().OnEvent(e); });

	struct Vertex
	{
		struct
		{
			float x, y, z;
		} Position;
		struct
		{
			float r, g, b;
		} Color;
	};

	const Vertex vertices[] = {
			{-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
			{0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f},
			{0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f},
			{-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f},

			{-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f},
			{0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
			{0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f},
			{-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
	};

	const unsigned indices[] = {
			0,1,2, 2,3,0,
			5,4,7, 7,6,5,
			3,2,6, 6,7,3,
			4,5,1, 1,0,4,
			0,3,7, 7,4,0,
			2,1,5, 5,6,2,
	};

	std::unique_ptr<VertexBuffer> vb = VertexBuffer::Create(vertices, sizeof(vertices), {
																																													{VertexLayout::Attribute::T::Float3},
																																													{VertexLayout::Attribute::T::Float3},
																																											});
	std::unique_ptr<IndexBuffer> ib = IndexBuffer::Create(indices, std::size(indices));

	m_Va = VertexArray::Create(std::move(vb), std::move(ib));

	m_Shdr = Shader::Create("Assets/Shaders/BasicShader.glsl");
	m_Shdr->Bind();

	m_Camera = std::make_unique<PerspectiveCamera>(
		PerspectiveCamera::Position{2.0f,2.0f,2.0f},
		PerspectiveCamera::Orientation{-nic::PI/4,nic::PI/4},
		s_Fov,
		float(m_Window->GetWidth()) / float(m_Window->GetHeight())
	);

	m_Shdr->SetUniformMat4("u_ModelTransform", glm::mat4(1.0f));

	m_Running = true;
}

int App::Run()
{
	NIC_PROFILE_FUNCTION();

	auto &window = *Get().m_Window;

	Renderer::Init();
	ImGuiLayer::Init(reinterpret_cast<GLFWwindow *>(window.GetNativeWindow()), glsl_version);

	auto &running = Get().m_Running;

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

	auto &va = *Get().m_Va;

	auto &dt = Get().m_Dt;
	std::chrono::time_point<std::chrono::high_resolution_clock> tp = std::chrono::high_resolution_clock::now();
	while (running)
	{
		NIC_PROFILE_SCOPE("Run Loop");
		dt = (std::chrono::high_resolution_clock::now() - tp).count() / 1000000000.0f;
		tp = std::chrono::high_resolution_clock::now();
		DoFrame(dt, window, va);
	}
	cameraCtrlThread.join();

	Renderer::Shutdown();
	ImGuiLayer::Shutdown();

	return 0;
}

void App::DoFrame(float dt, class Window &window, const class VertexArray &vertexArray)
{
	NIC_PROFILE_FUNCTION();

	// input stuff
	Get().m_Camera->SetFOV(s_Fov);
	Get().m_Camera->Bind(*Get().m_Shdr.get());
	UpdateImGuiLayerState(Get().m_ImGuiLayerShouldToggle);

	ImGuiLayer::BeginFrame();
	bool vSync = window.IsVSync();
	bool fullScreen = window.IsFullScreen();
	ImGuiLayer::Update(dt, s_Fov, vSync, fullScreen);

	Renderer::BeginScene();
	for (size_t i = 0; i < 40; i++)
	{
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), {1.5f * i, -1.5f * i, -1.5f * i});
		for (size_t j = 0; j < 40; j++)
		{
			Renderer::Draw(vertexArray);
			Get().m_Shdr->SetUniformMat4("u_ModelTransform", translation = glm::translate(translation, {-1.5f, -1.5f, -1.5f}));
		}
	}
	Get().m_Shdr->SetUniformMat4("u_ModelTransform", glm::mat4(1.0f));
	Renderer::EndScene();

	ImGuiLayer::EndFrame();

	window.Update(vSync, fullScreen);
}

void App::ControllCamera(float dt)
{
	NIC_PROFILE_FUNCTION(); // ~ 1,505ms Debug | 0,558 Release - InputPooling with glfw is heavy

	auto &camera = *Get().m_Camera;

	if (Input::IsKeyPressed(GLFW_KEY_W))
		camera.TransformPosition(0.0f,0.0f,-dt);
	else if (Input::IsKeyPressed(GLFW_KEY_S))
		camera.TransformPosition(0.0f,0.0f, dt);
	if (Input::IsKeyPressed(GLFW_KEY_D))
		camera.TransformPosition( dt,0.0f,0.0f);
	else if (Input::IsKeyPressed(GLFW_KEY_A))
		camera.TransformPosition(-dt,0.0f,0.0f);
	if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		camera.TransformPosition(0.0f, dt,0.0f);
	else if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		camera.TransformPosition(0.0f,-dt,0.0f);

	if (Input::IsKeyPressed(GLFW_KEY_UP))
		camera.TransformOrientation(-dt,0.0f);
	else if (Input::IsKeyPressed(GLFW_KEY_DOWN))
		camera.TransformOrientation( dt,0.0f);
	if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
		camera.TransformOrientation(0.0f,-dt);
	else if (Input::IsKeyPressed(GLFW_KEY_LEFT))
		camera.TransformOrientation(0.0f, dt);
}

void App::ShutDown()
{
	NIC_PROFILE_FUNCTION();

	Get().m_Running = false;
}

void App::UpdateImGuiLayerState(bool shouldToggle)
{
	if (shouldToggle)
	{
		DebugLog("Toggling ImGuiLayer\n");
		NIC_PROFILE_SCOPE("Toggling ImGuiLayer");

		ImGuiLayer::Toggle(reinterpret_cast<GLFWwindow *>(Get().m_Window->GetNativeWindow()), glsl_version);
		Get().m_ImGuiLayerShouldToggle = false;
	}
}

void App::OnEvent(Event &e)
{
	NIC_PROFILE_FUNCTION();

	e.Dispatch<CloseEvent>([](CloseEvent &e) {
		NIC_PROFILE_SCOPE("App CloseEvent dispatch");
		App::ShutDown();
		return true;
	});
	if (e.IsHandled())
		return;

	e.Dispatch<ResizeEvent>([](ResizeEvent &e) {
		NIC_PROFILE_SCOPE("App ResizeEvent dispatch");
		if (e.Height && e.Width)
		{
			Renderer::SetViewport(e.Width, e.Height);
			Get().m_Camera->SetAspectRatio(float(e.Width) / float(e.Height));
#ifdef PLATFORM_WINDOWS
			DoFrame(Get().m_Dt, *Get().m_Window, *Get().m_Va);
#endif
			return true;
		}
		return false;
	});

	e.Dispatch<KeyEvent>([](KeyEvent &e) {
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

Window &App::GetWindow() const
{
	NIC_PROFILE_FUNCTION();

	return *m_Window;
}