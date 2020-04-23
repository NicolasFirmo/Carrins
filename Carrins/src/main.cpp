#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

const char *glsl_version = "#version 410";

#ifndef NDEBUG
static constexpr const char *s_WindowTitle = "DEBUG";
struct
{
	float r = 0.0f;
	float g = 0.5f;
	float b = 1.0f;
	float a = 1.0f;
} static s_ClearColor;
#else
static constexpr const char *s_WindowTitle = "NDEBUG";
struct
{
	float r = 1.0f;
	float g = 0.5f;
	float b = 0.0f;
	float a = 1.0f;
} static s_ClearColor;
#endif

#ifndef PLATFORM_WINDOWS
#define DebugLog(x) std::cout << x << '\n'
int main()
#else
#define DebugLog(x) OutputDebugString(x)
int CALLBACK WinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow)
#endif
{
	GLFWwindow *window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	// TODO: glfwSetErrorCallback(my error callback);

	/* Set windows hints */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, s_WindowTitle, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return -1;

	DebugLog(glGetString(GL_VERSION));

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle &style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	GLuint va;
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	struct Vertex
	{
		struct
		{
			float x, y;
		} Pos;
	};

	const Vertex vertices[] = {
			{
					-0.5f,
					-0.5f,
			},
			{
					0.5f,
					-0.5f,
			},
			{
					0.5f,
					0.5f,
			},
			{
					-0.5f,
					0.5f,
			}};

	GLuint vb;
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // enable first atrib of the vertex
	glVertexAttribPointer(0, sizeof(Vertex::Pos) / sizeof(float), GL_FLOAT, GL_FALSE,
												sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Vertex::Pos)));

	const unsigned indices[] = {
			0, 1, 2, 2, 3, 0};

	GLuint ib;
	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glClearColor(s_ClearColor.r, s_ClearColor.g, s_ClearColor.b, s_ClearColor.a);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Poll for and process events */
		glfwPollEvents();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::Begin("Test");
		ImGui::Text("Texto de teste");
		ImGui::End();

		ImGui::Begin("Test2");
		ImGui::Text("textão");
		ImGui::End();

		glDrawElements(GL_TRIANGLES, std::size(indices), GL_UNSIGNED_INT, nullptr);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow *backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
	}

	// Cleanup
	glDeleteBuffers(1, &vb);
	glDeleteBuffers(1, &ib);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}