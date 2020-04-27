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

#include <glm/gtc/matrix_transform.hpp> // Projection Matrices

App App::s_Instance;

static float s_Fov = glm::radians(80.0f);

App &App::Get()
{
	return s_Instance;
}

App::App() : m_Window(Window::Create(640, 480, "Carrins"))
{
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
			0,1,2,	2,3,0,
			5,4,7,	7,6,5,
			3,2,6,	6,7,3,
			4,5,1,	1,0,4,
			0,3,7,	7,4,0,
			2,1,5,	5,6,2,
	};

	std::unique_ptr<VertexBuffer> vb = VertexBuffer::Create(vertices, sizeof(vertices), {
																																													{VertexLayout::Attribute::T::Float3},
																																													{VertexLayout::Attribute::T::Float3},
																																											});
	std::unique_ptr<IndexBuffer> ib = IndexBuffer::Create(indices, std::size(indices));

	m_Va = VertexArray::Create(std::move(vb), std::move(ib));

	m_Shdr = Shader::Create("Assets/Shaders/BasicShader.glsl");
	m_Shdr->Bind();

	m_Projection = glm::perspective(s_Fov, float(m_Window->GetWidth()) / float(m_Window->GetHeight()), 0.1f, 150.0f);
	m_View = glm::translate(m_View, {0.0f, 0.0f, -3.0f});
	m_View = glm::rotate(m_View, glm::pi<float>()/4, {1.0f, 0.0f, 0.0f});
	m_View = glm::rotate(m_View, -glm::pi<float>()/4, {0.0f, 1.0f, 0.0f});

	m_Shdr->SetUniformMat4("u_ViewProjection", m_Projection * m_View);
	m_Shdr->SetUniformMat4("u_ModelTransform", glm::mat4(1.0f));

	m_Running = true;
}

int App::Run()
{
	auto &window = *Get().m_Window;
	auto &va = *Get().m_Va;

	Renderer::Init();
	ImGuiLayer::Init(reinterpret_cast<GLFWwindow *>(window.GetNativeWindow()), glsl_version);

	auto &dt = Get().m_Dt;
	std::chrono::time_point<std::chrono::high_resolution_clock> tp = std::chrono::high_resolution_clock::now();

	auto &running = Get().m_Running;
	while (running)
	{
		dt = (std::chrono::high_resolution_clock::now() - tp).count()/1000000000.0f;
		tp = std::chrono::high_resolution_clock::now();
		DoFrame(dt, window, va);
	}

	Renderer::Shutdown();
	ImGuiLayer::Shutdown();

	return 0;
}

void App::DoFrame(float dt, const class Window &window, const class VertexArray &vertexArray)
{
	// input stuff
	ControllCamera(dt);

	ImGuiLayer::BeginFrame();
	ImGuiLayer::Update(dt, s_Fov);

	Renderer::BeginScene();
	Renderer::Draw(vertexArray);
	Renderer::EndScene();

	ImGuiLayer::EndFrame();
	window.Update();
}

void App::ControllCamera(float dt)
{
	auto& view = Get().m_View;

	if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		dt *= 0.5f;
	
	if (Input::IsKeyPressed(GLFW_KEY_W))
	{
		view = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, dt}) * view;
		App::UpdateViewProjection();
	}
	else if (Input::IsKeyPressed(GLFW_KEY_S))
	{
		view = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, -dt}) * view;
		App::UpdateViewProjection();
	}
	if (Input::IsKeyPressed(GLFW_KEY_D))
	{
		view = glm::translate(glm::mat4(1.0f), {-dt, 0.0f, 0.0f}) * view;
		App::UpdateViewProjection();
	}
	else if (Input::IsKeyPressed(GLFW_KEY_A))
	{
		view = glm::translate(glm::mat4(1.0f), {dt, 0.0f, 0.0f}) * view;
		App::UpdateViewProjection();
	}

	if (Input::IsKeyPressed(GLFW_KEY_Q))
	{
		view = glm::rotate(glm::mat4(1.0f),-dt, {0.0f, 0.0f, 1.0f}) * view;
		App::UpdateViewProjection();
	}
	else if (Input::IsKeyPressed(GLFW_KEY_E))
	{
		view = glm::rotate(glm::mat4(1.0f), dt, {0.0f, 0.0f, 1.0f}) * view;
		App::UpdateViewProjection();
	}
	if (Input::IsKeyPressed(GLFW_KEY_UP))
	{
		view = glm::rotate(glm::mat4(1.0f), dt, {1.0f, 0.0f, 0.0f}) * view;
		App::UpdateViewProjection();
	}
	else if (Input::IsKeyPressed(GLFW_KEY_DOWN))
	{
		view = glm::rotate(glm::mat4(1.0f), -dt, {1.0f, 0.0f, 0.0f}) * view;
		App::UpdateViewProjection();
	}
	if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
	{
		view = glm::rotate(glm::mat4(1.0f), dt, {0.0f, 1.0f, 0.0f}) * view;
		App::UpdateViewProjection();
	}
	else if (Input::IsKeyPressed(GLFW_KEY_LEFT))
	{
		view = glm::rotate(glm::mat4(1.0f), -dt, {0.0f, 1.0f, 0.0f}) * view;
		App::UpdateViewProjection();
	}
}

void App::ShutDown()
{
	Get().m_Running = false;
}

void App::UpdateViewProjection()
{
	Get().m_Projection = glm::perspective(s_Fov, float(Get().m_Window->GetWidth()) / float(Get().m_Window->GetHeight()), 0.1f, 150.0f);
	Get().m_Shdr->SetUniformMat4("u_ViewProjection", Get().m_Projection * Get().m_View);
}

void App::OnEvent(Event &e)
{
	if (e.Dispatch<CloseEvent>([](CloseEvent &e) {
				App::ShutDown();
				return true;
			}))
		return;

	e.Dispatch<ResizeEvent>([](ResizeEvent &e) {
		Renderer::SetViewport(e.Width, e.Height);
		Get().m_Projection = glm::perspective(s_Fov, float(e.Width) / float(e.Height), 0.1f, 150.0f);
		Get().m_Shdr->SetUniformMat4("u_ViewProjection", Get().m_Projection * Get().m_View);
#ifdef PLATFORM_WINDOWS
		DoFrame(*Get().m_Window, *Get().m_Va);
#endif
		return false;
	});

	e.Dispatch<KeyEvent>([](KeyEvent &e) {
		if (e.Code == GLFW_KEY_ESCAPE)
		{
			App::ShutDown();
			DebugLog("Application closed on esc\n");
			return true;
		}
		else
			return false;
	});

	if (e.IsHandled())
		return;
}

Window &App::GetWindow() const
{
	return *m_Window;
}