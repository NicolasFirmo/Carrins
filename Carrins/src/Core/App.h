#pragma once
#include "Window.h"

class App
{
public:
	static App &Get();
	~App();

	static void Init();
	static int Run();
	static void ShutDown();

	static void DoFrame(float dt);

	static void ControllCamera(float dt);
	static void UpdateImGuiLayerState(bool shouldToggle);

	void OnEvent(class Event &e);

	Window &GetWindow() const;

private:
	App();
	App(const App &) = delete;

private:
	static App s_Instance;

	std::unique_ptr<Window> m_Window;
	std::unique_ptr<class PerspectiveCamera> m_Camera;
	std::unique_ptr<class Renderer> m_Renderer;

	bool m_Running = false;
	bool m_ImGuiLayerShouldToggle = false;
	float m_Dt = 0.0f;
};