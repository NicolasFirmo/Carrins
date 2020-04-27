#include "OpenGLRendererAPI.h"
#include "OpenGLCore.h"

#include "Core/Renderer/Renderer.h"

std::unique_ptr<RendererAPI> Renderer::s_API = std::make_unique<OpenGLRendererAPI>();

void OpenGLRendererAPI::SetClearColor(const float r, const float g, const float b, const float a) const
{
	GLCall(glClearColor(r, g, b, a));
}
void OpenGLRendererAPI::SetViewport(int width,int height) const
{
	GLCall(glViewport(0,0,width,height));
}
void OpenGLRendererAPI::Draw(const VertexArray &vertexArray) const
{
	GLCall(glDrawElements(GL_TRIANGLES, vertexArray.GetIndexCount(), GL_UNSIGNED_INT, nullptr));
}
void OpenGLRendererAPI::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}