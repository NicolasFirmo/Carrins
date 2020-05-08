#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

class GameLayer
{
public:
	struct Object
	{
		glm::vec3 Position;
		glm::vec3 Velocity;
		const float RestitutionFactor;
	};
	struct Plane
	{
		glm::vec3 Position;
		glm::vec3 Normal;
	};

public:
	static constexpr float c_G = -9.8f;
	static constexpr float c_ResThresh = 0.005f;

public:
	static GameLayer &Get();

	static void Init(float aspectRatio);
	static void Shutdown();

	static void OnUpdate(float dt);
	static void OnRender();

	static void OnEvent(class Event &e);

	static void ControllCamera(float dt);

	static void ResolveColision(Object &obj, const Plane &colisionPlane);

private:
	GameLayer() = default;
	GameLayer(const GameLayer &) = delete;

private:
	static GameLayer s_Instance;
	bool m_Running = false;

	std::unique_ptr<std::thread> m_cameraCtrlThread;
	std::unique_ptr<class PerspectiveCamera> m_Camera;
	std::unique_ptr<class Renderer> m_Renderer;

	std::unique_ptr<class Track> m_Track;

	Object m_Obj = {{0.5f, 2.0f, -0.5f}, {0.0f, 0.0f, 0.0f}, 0.90f};
	Plane m_TestGround = {{0.0f, 0.0f, 0.0f}, glm::normalize(glm::vec3{0.0f, 10.0f, 1.0f})};
	Plane m_TestWall = {{0.0f, 0.0f, 4.0f}, glm::normalize(glm::vec3{0.0f, 1.0f, -10.0f})};
};