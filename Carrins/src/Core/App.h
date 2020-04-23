#pragma once

class App
{
public:
	App(const App&) = delete;
	static App& Get();

	static int Run();
private:
	App();
private:
	static App s_Instance;
	
	std::unique_ptr<class Window> m_Window;
};