#pragma once
#include "Window.h"

class App
{
public:
	App(const App &) = delete;
	static App &Get();

	static int Run();
	static void ShutDown();

	void OnEvent(class Event &e);

	class Window &GetWindow() const;

private:
	App();

private:
	static App s_Instance;
	bool m_Running = false;

	std::unique_ptr<class Window> m_Window;

	std::unique_ptr<class VertexArray> m_Va;
	std::unique_ptr<class Shader> m_Shdr;
};