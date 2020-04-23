#include "glfwWindow.h"
#include <GLFW/glfw3.h>

#include "Core.h"

#include "Platform/OpenGL/OpenGLContext.h"

std::unique_ptr<Window> Window::Create(int width, int height, const char *title)
{
	return std::make_unique<glfwWindow>(width, height, title);
}

glfwWindow::glfwWindow(int width, int height, const char *title)
{
	NIC_ASSERT(glfwInit(), "glfw not Initialized");

	glfwSetErrorCallback([](int code, const char *msg) {
		DebugLog("GLFW ERROR " << code << ": " << msg << '\n');
	});

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	NIC_ASSERT(m_Window = glfwCreateWindow(width, height, title, NULL, NULL), "window not created");

	glfwMakeContextCurrent(m_Window);
	OpenGLContext::Init(reinterpret_cast<void*>(glfwGetProcAddress));

	EnableVSync();
}
glfwWindow::~glfwWindow()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void *glfwWindow::GetNativeWindow()
{
	return m_Window;
}

void glfwWindow::EnableVSync()
{
	glfwSwapInterval(1);
}
void glfwWindow::DisableVSync()
{
	glfwSwapInterval(0);
}

void glfwWindow::PollEvents()
{
	glfwPollEvents();
}
void glfwWindow::SwapBuffers()
{
	glfwSwapBuffers(m_Window);
}

bool glfwWindow::WindowShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}