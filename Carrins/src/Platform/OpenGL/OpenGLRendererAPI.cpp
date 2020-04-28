#include "OpenGLRendererAPI.h"
#include "OpenGLCore.h"

#include "Core/Renderer/Renderer.h"

#include "Instrumentation/Profile.h"

std::unique_ptr<RendererAPI> Renderer::s_API = std::make_unique<OpenGLRendererAPI>();

void OpenGLRendererAPI::SetClearColor(const float r, const float g, const float b, const float a) const
{
	NIC_PROFILE_FUNCTION();

	GLCall(glClearColor(r, g, b, a));
}
void OpenGLRendererAPI::SetViewport(int width,int height) const
{
	NIC_PROFILE_FUNCTION();

	GLCall(glViewport(0,0,width,height));
}
void OpenGLRendererAPI::EnableFaceCulling(WindingOrder order) const
{
	NIC_PROFILE_FUNCTION();

	GLCall(glEnable(GL_CULL_FACE));
	switch (order)
	{
	case WindingOrder::Clockwise:
		GLCall(glFrontFace(GL_CW));
		break;
	case WindingOrder::CounterClockwise:
		GLCall(glFrontFace(GL_CCW));
		break;
	}
}
void OpenGLRendererAPI::EnableDepthTesting() const
{
	NIC_PROFILE_FUNCTION();

	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LESS)); // Not Necessary but lets be explicit
}
void OpenGLRendererAPI::Draw(const VertexArray &vertexArray) const
{
	NIC_PROFILE_FUNCTION();

	GLCall(glDrawElements(GL_TRIANGLES, vertexArray.GetIndexCount(), GL_UNSIGNED_INT, nullptr));
}
void OpenGLRendererAPI::Clear() const
{
	NIC_PROFILE_FUNCTION();

	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}