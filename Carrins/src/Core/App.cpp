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

App App::s_Instance;

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
			float x, y;
		} Position;
		struct
		{
			float r, g, b;
		} Color;
	};

	const Vertex vertices[] = {
			{-0.5f,-0.5f,0.5f,1.0f,0.0f},
			{ 0.5f,-0.5f,0.0f,1.0f,0.0f},
			{ 0.5f, 0.5f,0.0f,1.0f,0.5f},
			{-0.5f, 0.5f,0.0f,1.0f,0.0f},
	};

	const unsigned indices[] = {
			0,1,2,	2,3,0,
	};

	std::unique_ptr<VertexBuffer> vb = VertexBuffer::Create(vertices, sizeof(vertices), {
		{VertexLayout::Attribute::T::Float2, "a_Position"},
		{VertexLayout::Attribute::T::Float3, "a_Color"},
		});
	std::unique_ptr<IndexBuffer> ib = IndexBuffer::Create(indices, std::size(indices));

	m_Va = VertexArray::Create(std::move(vb), std::move(ib));

	m_Shdr = Shader::Create("Assets/Shaders/BasicShader.glsl");
	m_Shdr->Bind();

	m_Running = true;
}

int App::Run()
{
	auto &window = *Get().m_Window;
	auto &va = *Get().m_Va;

	Renderer::Init();
	ImGuiLayer::Init(reinterpret_cast<GLFWwindow *>(window.GetNativeWindow()), glsl_version);

	auto &running = Get().m_Running;
	while (running)
		DoFrame(window,va);

	Renderer::Shutdown();
	ImGuiLayer::Shutdown();

	return 0;
}

void App::DoFrame(const class Window& window, const class VertexArray& vertexArray)
{
	ImGuiLayer::BeginFrame();
	ImGuiLayer::Update();

	Renderer::BeginScene();
	Renderer::Draw(vertexArray);
	Renderer::EndScene();

	ImGuiLayer::EndFrame();
	window.Update();
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
		
	e.Dispatch<ResizeEvent>([](ResizeEvent &e) {
		Renderer::SetViewport(e.Width,e.Height);
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