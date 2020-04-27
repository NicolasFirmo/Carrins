#include "glfwInput.h"
#include <GLFW/glfw3.h>
#include "Core/App.h"

std::unique_ptr<Input> Input::s_Instance = std::make_unique<glfwInput>();

bool glfwInput::IsKeyPressed_(int keycode) const
{
	auto window = static_cast<GLFWwindow *>(App::Get().GetWindow().GetNativeWindow());

	auto state = glfwGetKey(window, keycode);

	return state != GLFW_RELEASE;
}
bool glfwInput::IsMouseButtonPressed_(int button) const
{
	auto window = static_cast<GLFWwindow *>(App::Get().GetWindow().GetNativeWindow());

	auto state = glfwGetMouseButton(window, button);

	return state != GLFW_RELEASE;
}

Input::Pos glfwInput::GetMousePosition_() const
{
	auto window = static_cast<GLFWwindow *>(App::Get().GetWindow().GetNativeWindow());

	double posX, posY;
	glfwGetCursorPos(window, &posX, &posY);

	return {(float)posX, (float)posY};
}
float glfwInput::GetMouseXPosition_() const
{
	return GetMousePosition_().X;
}
float glfwInput::GetMouseYPosition_() const
{
	return GetMousePosition_().Y;
}