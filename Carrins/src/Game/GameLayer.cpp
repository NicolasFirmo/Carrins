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
#include "Utility/NumberManipulation.hpp"

#include "Instrumentation/Profile.h"

GameLayer GameLayer::s_Instance;

GameLayer& GameLayer::Get()
{
	return s_Instance;
}

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

	for (size_t i = 0; i < 300; i++)
		for (size_t k = 0; k < 300; k++)
			cubes.emplace_back(1.0f, 0.166666667f, 0.8f, glm::vec3{ -1.0f * k, 1.0f * (2 * k + i), -1.0f * i }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f });

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

	for (auto& cube : cubes)
	{
		if (cube.GetPosition().y <= 0.5f && cube.GetLinearVelocity().y < 0)
		{
			//DebugLog("Before colision:\nPos:" << cube.GetPosition().y << "\nVel: " << cube.GetLinearVelocity().y << '\n');
			cube.Colide(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ cube.GetPosition().x, 0.0f, cube.GetPosition().z });
			cube.Update(dt);
			//DebugLog("After colision:\nPos:" << cube.GetPosition().y << "\nVel: " << cube.GetLinearVelocity().y << '\n');
		}
		else if (cube.GetPosition().y > 0.5f + c_ResThresh)
		{
			cube.Drag(glm::vec3{ 0.0f, c_G * dt,0.0f });
			cube.Update(dt);
		}
		else
		{
			if (glm::length(cube.GetLinearVelocity()) < 0.1f)
				cube.Drag(-cube.GetLinearVelocity());
			cube.Update(dt);
		}
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
		renderer.DrawObject(glm::translate(glm::mat4(1.0f), cube.GetPosition()));

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
