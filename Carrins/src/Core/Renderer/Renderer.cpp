#include "Renderer.h"
#include "RendererAPI.h"

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

void Renderer::Init()
{
	s_API->SetClearColor(s_ClearColor.r, s_ClearColor.g, s_ClearColor.b, s_ClearColor.a);
}

void Renderer::BeginScene()
{
	s_API->Clear();
}
void Renderer::EndScene()
{
}