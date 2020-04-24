#pragma once

class App
{
public:
	App(const App&) = delete;
	static App& Get();

	static int Run();
	static void ShutDown();

	void OnEvent(class Event& e);
private:
	App();
private:
	static App s_Instance;
	bool m_Running = false;
	
	std::unique_ptr<class Window> m_Window;
};