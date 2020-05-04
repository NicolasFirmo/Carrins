#include "GameLayer.h"
#include "Core.h"

#include "Track.h"

#include "Core/Inupt.h"

#include "Core/Renderer/Renderer.h"

#include "Core/Events/WindowEvent.h"
#include "Core/Events/KeyboardEvent.h"
#include "Core/Events/MouseEvent.h"

#include "ImGui/ImGuiLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Utility/MathConstants.hpp"
#include "Utility/NumberManipulation.hpp"

#include "Instrumentation/Profile.h"

GameLayer GameLayer::s_Instance;

GameLayer &GameLayer::Get()
{
	return s_Instance;
}

void GameLayer::Init(float aspectRation)
{
	NIC_PROFILE_FUNCTION();

	Get().m_Running = true;

	Get().m_Camera = std::make_unique<PerspectiveCamera>(
			PerspectiveCamera::Position{2.0f, 2.0f, 2.0f},
			PerspectiveCamera::Orientation{-nic::PI / 4, nic::PI / 4},
			nic::PI / 3,
			aspectRation);

	Get().m_Track = std::make_unique<Track>("Test");

	Get().m_Renderer = std::make_unique<Renderer>("Cubo.obj", "BasicShader.glsl");

	Get().m_cameraCtrlThread = std::make_unique<std::thread>([]() {
		auto &running = Get().m_Running;
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
	Get().m_T = nic::Wrap(Get().m_T + dt, nic::PI * 2, 0);
}

void GameLayer::OnRender()
{
	NIC_PROFILE_FUNCTION();

	auto &renderer = *Get().m_Renderer;
	auto &camera = *Get().m_Camera;
	auto t = Get().m_T;

	Get().m_Track->Draw(camera);

	renderer.BeginScene(camera);

	for (int i = -3; i < 3; i++)
		for (int j = 1; j < 3; j++)
			for (int k = -3; k < 3; k++)
				renderer.DrawObject(glm::translate(glm::mat4(1.0f), {4.0f * i, 4.0f * j, 4.0f * k}) * glm::inverse(glm::eulerAngleYXZ(float(i * nic::PI / 8 + t), float(j * nic::PI / 8), float(k * nic::PI / 8))));

	renderer.EndScene();
}

void GameLayer::OnEvent(Event &e)
{
	NIC_PROFILE_FUNCTION();

	e.Dispatch<ResizeEvent>([](ResizeEvent &e) {
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
	auto &camera = *Get().m_Camera;

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
