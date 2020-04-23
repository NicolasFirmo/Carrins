#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

#include "Core.h"

#include "Core/Renderer/Renderer.h"

#ifndef NDEBUG
	#define GLCall(x) while(glGetError()); x; while(auto error = glGetError())NIC_ASSERT(false, "[OpenGL error] (0x0" << std::hex << error << std::dec << "): " << #x)
#else
	#define GLCall(x) x
#endif

std::unique_ptr<RendererAPI> Renderer::s_API = std::make_unique<OpenGLRendererAPI>();

void OpenGLRendererAPI::SetClearColor(const float r, const float g, const float b, const float a) 
{
	GLCall(glClearColor(r, g, b, a));
}
void OpenGLRendererAPI::Clear() 
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}