#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Solid.h"

class GameLayer
{
public:
	static constexpr float c_G = -9.8f;
	static constexpr float c_ResThresh = 0.005f;
	static float s_COR;

public:
	static GameLayer& Get();

	static void Init(float aspectRatio);
	static void Shutdown();

	static void OnUpdate(float dt);
	static void OnRender();

	static void OnEvent(class Event& e);

	static void ControllCamera(float dt);

private:
	GameLayer() = default;
	GameLayer(const GameLayer&) = delete;

private:
	static GameLayer s_Instance;
	bool m_Running = false;

	std::unique_ptr<std::thread> m_cameraCtrlThread;
	std::unique_ptr<class PerspectiveCamera> m_Camera;
	std::unique_ptr<class Renderer> m_Renderer;

	std::unique_ptr<class Track> m_Track;

	std::vector<Solid> m_Cubes;
};