#pragma once
#include "InputCodes.h"

class Input
{
public:
	struct Pos
	{
		float X, Y;
	};

public:
	static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressed_(keycode); }
	static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressed_(button); }

	static Pos GetMousePosition() { return s_Instance->GetMousePosition_(); }
	static float GetMouseXPosition() { return s_Instance->GetMouseXPosition_(); }
	static float GetMouseYPosition() { return s_Instance->GetMouseYPosition_(); }

protected:
	virtual bool IsKeyPressed_(int keycode) const = 0;
	virtual bool IsMouseButtonPressed_(int button) const = 0;

	virtual Pos GetMousePosition_() const = 0;
	virtual float GetMouseXPosition_() const = 0;
	virtual float GetMouseYPosition_() const = 0;

protected:
	Input() = default;
	Input(const Input&) = delete;

private:
	static std::unique_ptr<Input> s_Instance;
};