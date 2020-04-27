#include "glfwWindow.h"
#include <GLFW/glfw3.h>

#include "Core.h"

#include "Core/Events/WindowEvent.h"
#include "Core/Events/KeyboardEvent.h"
#include "Core/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

std::unique_ptr<Window> Window::Create(int width, int height, const char* title)
{
	return std::make_unique<glfwWindow>(width, height, title);
}

glfwWindow::glfwWindow(int width, int height, const char* title) : m_WindowData({ width, height })
{
	NIC_ASSERT(glfwInit(), "glfw not Initialized");

	glfwSetErrorCallback([](int code, const char* msg) {
		DebugLog("GLFW ERROR " << code << ": " << msg << '\n');
		});

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	NIC_ASSERT(m_Window = glfwCreateWindow(width, height, title, NULL, NULL), "window not created");
	m_FullScreen = false;

	glfwMakeContextCurrent(m_Window);
	OpenGLContext::Init(reinterpret_cast<void*>(glfwGetProcAddress));

	glfwSwapInterval(1); // VSync on
	m_VSync = true;

	// Set Callbacks
	glfwSetWindowUserPointer(m_Window, &m_WindowData);

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
		Data& data = *reinterpret_cast<Data*>(glfwGetWindowUserPointer(window));

		CloseEvent e;
		data.EventCallback(e);
		});
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		Data& data = *reinterpret_cast<Data*>(glfwGetWindowUserPointer(window));
		data.Width = width;
		data.Height = height;

		ResizeEvent e{ width, height };
		data.EventCallback(e);
		});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		Data& data = *reinterpret_cast<Data*>(glfwGetWindowUserPointer(window));

		switch (action)
		{
		case GLFW_PRESS:
		{
			KeyEvent e{ KeyEvent::T::Pressed, key };
			data.EventCallback(e);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyEvent e{ KeyEvent::T::Released, key };
			data.EventCallback(e);
			break;
		}
		case GLFW_REPEAT:
		{
			KeyEvent e{ KeyEvent::T::Repeated, key };
			data.EventCallback(e);
			break;
		}
		}
		});
	glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned keycode) {
		Data& data = *reinterpret_cast<Data*>(glfwGetWindowUserPointer(window));

		CharEvent e{ keycode };
		data.EventCallback(e);
		});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double posX, double posY) {
		Data& data = *reinterpret_cast<Data*>(glfwGetWindowUserPointer(window));

		MouseMoveEvent e{ (float)posX, (float)posY };
		data.EventCallback(e);
		});
	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
		Data& data = *reinterpret_cast<Data*>(glfwGetWindowUserPointer(window));

		switch (action)
		{
		case GLFW_PRESS:
		{
			MouseButtonEvent e{ MouseButtonEvent::T::Pressed, button };
			data.EventCallback(e);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonEvent e{ MouseButtonEvent::T::Released, button };
			data.EventCallback(e);
			break;
		}
		}
		});
	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double deltaX, double deltaY) {
		Data& data = *reinterpret_cast<Data*>(glfwGetWindowUserPointer(window));

		MouseScrollEvent e{ (float)deltaX, (float)deltaY };
		data.EventCallback(e);
		});
}
glfwWindow::~glfwWindow()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void glfwWindow::SetEventCallback(const EventCallbackFn& callback)
{
	m_WindowData.EventCallback = callback;
}

void* glfwWindow::GetNativeWindow() const
{
	return m_Window;
}

int glfwWindow::GetWidth() const
{
	return m_WindowData.Width;
}
int glfwWindow::GetHeight() const
{
	return m_WindowData.Height;
}

bool glfwWindow::IsFullScreen() const
{
	return m_FullScreen;
}
void glfwWindow::ToggleFullScreen()
{
	if (m_FullScreen)
	{
		m_FullScreen = false;
		DebugLog("Going Window\n");
		glfwSetWindowMonitor(m_Window, nullptr, 40, 40, m_WindowLastWidth, m_WindowLastHeight, GLFW_DONT_CARE);
	}
	else
	{
		m_FullScreen = true;
		DebugLog("Going Full\n");
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		m_WindowLastWidth = m_WindowData.Width;
		m_WindowLastHeight = m_WindowData.Height;
		glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
		if (m_VSync)
			glfwSwapInterval(1);
	}
}

bool glfwWindow::IsVSync() const
{
	return m_VSync;
}
void glfwWindow::ToggleVSync()
{
	if (m_VSync)
	{
		m_VSync = false;
		glfwSwapInterval(0);
	}
	else
	{
		m_VSync = true;
		glfwSwapInterval(1);
	}
}

void glfwWindow::Update(bool vSync, bool fullScreen)
{
	if (IsVSync() != vSync)
		ToggleVSync();
	if (IsFullScreen() != fullScreen)
		ToggleFullScreen();

	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}