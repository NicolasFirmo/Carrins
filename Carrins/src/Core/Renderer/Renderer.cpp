#include "Renderer.h"
#include "RendererAPI.h"

#include "Bindables/VertexBuffer.h"
#include "Bindables/IndexBuffer.h"
#include "Bindables/VertexArray.h"
#include "Bindables/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // Projection Matrices

#include "Instrumentation/Profile.h"

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

std::unique_ptr<Shader> Renderer::s_Shader = nullptr;
std::unique_ptr<VertexArray> Renderer::s_Va = nullptr;
std::unique_ptr<IndexBuffer> Renderer::s_Ib = nullptr;

void Renderer::SetViewport(int width, int height)
{
	NIC_PROFILE_FUNCTION();

	s_API->SetViewport(width, height);
}

void Renderer::Init()
{
	NIC_PROFILE_FUNCTION();

	s_API->EnableDepthTesting();
	s_API->EnableFaceCulling(RendererAPI::WindingOrder::CounterClockwise);
	s_API->SetClearColor(s_ClearColor.r, s_ClearColor.g, s_ClearColor.b, s_ClearColor.a);

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

	std::unique_ptr<VertexBuffer> vb = VertexBuffer::Create(vertices, sizeof(vertices));
	s_Ib = IndexBuffer::Create(indices, std::size(indices));

	s_Va = VertexArray::Create(std::move(vb),{
						{VertexLayout::Attribute::T::Float3},
						{VertexLayout::Attribute::T::Float3},
					});

	s_Shader = Shader::Create("Assets/Shaders/BasicShader.glsl");
}
void Renderer::Shutdown()
{
	NIC_PROFILE_FUNCTION();

	s_Shader.release();
	s_Va.release();
	s_Ib.release();
}

void Renderer::BeginScene(const Camera& camera)
{
	NIC_PROFILE_FUNCTION();

	s_API->Clear();
	s_Shader->Bind();
	s_Va->Bind();
	s_Ib->Bind();

	camera.Bind(*s_Shader);
}

void Renderer::DrawCube(float x, float y, float z)
{
	NIC_PROFILE_FUNCTION();

	s_Shader->SetUniformMat4("u_ModelTransform", glm::translate(glm::mat4(1.0f),{x,y,z}));

	s_API->Draw(*s_Ib);
}

void Renderer::EndScene()
{
	NIC_PROFILE_FUNCTION();
}


