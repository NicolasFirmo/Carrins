#include "GameLayer.h"
#include "Core.h"

#include "Track.h"

#include "Core/Inupt.h"

#include "Core/Renderer/Renderer.h"

#include "Core/Events/WindowEvent.h"
#include "Core/Events/KeyboardEvent.h"
#include "Core/Events/MouseEvent.h"

#include "ImGui/ImGuiLayer.h"

#include "Utility/MathConstants.hpp"
#include "Utility/Manip.hpp"

#include "Instrumentation/Profile.h"

GameLayer GameLayer::s_Instance;

GameLayer& GameLayer::Get()
{
	return s_Instance;
}

float GameLayer::s_COR = 1.0f;

void GameLayer::Init(float aspectRatio)
{
	NIC_PROFILE_FUNCTION();

	Get().m_Running = true;

	Get().m_Camera = std::make_unique<PerspectiveCamera>(
		PerspectiveCamera::Position{ 2.0f, 2.0f, 2.0f },
		PerspectiveCamera::Orientation{ -float(nic::PI) / 4, float(nic::PI) / 4 },
		float(nic::PI) / 3,
		aspectRatio);

	Get().m_Track = std::make_unique<Track>("Test");

	Get().m_Renderer = std::make_unique<Renderer>("Cubo.obj", "BasicShader.glsl");

	Get().m_cameraCtrlThread = std::make_unique<std::thread>([]() {
		auto& running = Get().m_Running;
		float dt = 0.0f;
		std::chrono::time_point<std::chrono::high_resolution_clock> tp = std::chrono::high_resolution_clock::now();

		while (running)
		{
			NIC_PROFILE_SCOPE("Camera controller loop");
			dt = (std::chrono::high_resolution_clock::now() - tp).count() / 1000000000.0f;
			tp = std::chrono::high_resolution_clock::now();
			ControllCamera(dt);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		});

	auto& cubes = Get().m_Cubes;

	for (size_t i = 0; i < 142; i++)
		for (size_t k = 0; k < 142; k++)
			cubes.emplace_back(1.0f, 1.0f, 0.5f, 0.5f, 0.5f,
				glm::vec3{ -float(nic::SQRT3) * k, 1.5f + 1.0f * (2 * k + i), -float(nic::SQRT3) * i }, glm::quat{ 1.0f,0.0f,0.0f,0.0f },
				glm::vec3{ 0.0f, -0.1f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f });

}
void GameLayer::Shutdown()
{
	NIC_PROFILE_FUNCTION();

	Get().m_Running = false;
	Get().m_cameraCtrlThread->join();

	Get().m_cameraCtrlThread.release();
	Get().m_Camera.release();
	Get().m_Renderer.release();
}

void GameLayer::OnUpdate(float dt)
{
	NIC_PROFILE_FUNCTION();

	auto& cubes = Get().m_Cubes;

	static constexpr size_t subdivisionsPerFrame = 3;

	dt /= subdivisionsPerFrame;

	for (size_t i = 0; i < subdivisionsPerFrame; i++)
		for (auto& cube : cubes)
		{
			if (Input::IsKeyPressed(GLFW_KEY_P))
				cube.Thrust({ 0,20.0f * dt,0 }, { 0,-0.5,0.1f });
			if (Input::IsKeyPressed(GLFW_KEY_O))
				cube.Thrust({ 0,-20.0f * dt,0 }, { 0,-0.5,0.1f });
			if (Input::IsKeyPressed(GLFW_KEY_V))
				cube.Thrust({ 0,20.0f * dt,2.0f * dt }, { 0.2,-0.5,0.4 });
			if (Input::IsKeyPressed(GLFW_KEY_C))
				cube.Thrust({ 0,20.0f * -dt,2.0f * -dt }, { 0.2,-0.5,0.4 });

			cube.SetRestitutionFactor(s_COR);

			glm::vec3 colidedVerticeSum{ 0.0f, 0.0f, 0.0f };
			size_t colisionCount = 0;
			float depthFactor = 0.0f;
			for (auto&& vertice : cube.GetVertices())
				if (float depth = 0.0f - vertice.y; depth > 0.0f)
				{
					colidedVerticeSum += vertice * depth;
					depthFactor += depth;
				}

			if (depthFactor > 0.0f)
			{
				const auto contactPoint = colidedVerticeSum / depthFactor;

				NIC_ASSERT(contactPoint.y <= 0.0f, "Ponto de contato acima do ch�o");

				cube.Colide(glm::vec3{ 0.0f, 1.0f, 0.0f }, contactPoint, -contactPoint.y, 1.0f);
			}

			cube.TransformLinearVelocity(glm::vec3{ 0.0f, c_G * dt ,0.0f });
			cube.Update(dt);

		}
}

void GameLayer::OnRender()
{
	NIC_PROFILE_FUNCTION();

	auto& renderer = *Get().m_Renderer;
	auto& camera = *Get().m_Camera;
	auto& cubes = Get().m_Cubes;

	Get().m_Track->Draw(camera);

	renderer.BeginScene(camera);

	for (auto& cube : cubes)
		renderer.DrawObject(cube.GetTransfomation());

	renderer.EndScene();
}

void GameLayer::OnEvent(Event& e)
{
	NIC_PROFILE_FUNCTION();

	e.Dispatch<ResizeEvent>([](ResizeEvent& e) {
		NIC_PROFILE_SCOPE("App ResizeEvent dispatch");
		if (e.Height && e.Width)
		{
			Get().m_Camera->SetAspectRatio(float(e.Width) / float(e.Height));
			return true;
		}
		return false;
		});
}

void GameLayer::ControllCamera(float dt)
{
	auto& camera = *Get().m_Camera;

	if (Input::IsKeyPressed(GLFW_KEY_W))
		camera.TransformPosition(0.0f, 0.0f, -dt);
	else if (Input::IsKeyPressed(GLFW_KEY_S))
		camera.TransformPosition(0.0f, 0.0f, dt);
	if (Input::IsKeyPressed(GLFW_KEY_D))
		camera.TransformPosition(dt, 0.0f, 0.0f);
	else if (Input::IsKeyPressed(GLFW_KEY_A))
		camera.TransformPosition(-dt, 0.0f, 0.0f);
	if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		camera.TransformPosition(0.0f, dt, 0.0f);
	else if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		camera.TransformPosition(0.0f, -dt, 0.0f);

	if (Input::IsKeyPressed(GLFW_KEY_Q))
		camera.TransformOrientation(0.0f, 0.0f, dt);
	else if (Input::IsKeyPressed(GLFW_KEY_E))
		camera.TransformOrientation(0.0f, 0.0f, -dt);
	if (Input::IsKeyPressed(GLFW_KEY_UP))
		camera.TransformOrientation(dt, 0.0f, 0.0f);
	else if (Input::IsKeyPressed(GLFW_KEY_DOWN))
		camera.TransformOrientation(-dt, 0.0f, 0.0f);
	if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
		camera.TransformOrientation(0.0f, -dt, 0.0f);
	else if (Input::IsKeyPressed(GLFW_KEY_LEFT))
		camera.TransformOrientation(0.0f, dt, 0.0f);
}
