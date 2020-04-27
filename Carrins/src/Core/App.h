#pragma once
#include "Window.h"
#include <glm/glm.hpp>

class App
{
public:
	static App &Get();

	static int Run();
	static void DoFrame(float dt, class Window& window, const class VertexArray& vertexArray);
	static void ShutDown();

	static void UpdateViewProjection();
	static void ControllCamera(float dt);

	void OnEvent(class Event &e);

	class Window &GetWindow() const;

private:
	App();
	App(const App &) = delete;

private:
	static App s_Instance;

	bool m_Running = false;

	std::unique_ptr<class Window> m_Window;

	float m_Dt = 0.0f;

	std::unique_ptr<class VertexArray> m_Va;
	std::unique_ptr<class Shader> m_Shdr;

	// Camera
	glm::mat4 m_Projection = glm::mat4(1.0f);
	glm::mat4 m_View = glm::mat4(1.0f);
};