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
	static bool IsKeyPressed(int keycode) { return s_Instatnce->IsKeyPressed_(keycode); }
	static bool IsMouseButtonPressed(int button) { return s_Instatnce->IsMouseButtonPressed_(button); }

	static Pos GetMousePosition() { return s_Instatnce->GetMousePosition_(); }
	static float GetMouseXPosition() { return s_Instatnce->GetMouseXPosition_(); }
	static float GetMouseYPosition() { return s_Instatnce->GetMouseYPosition_(); }

protected:
	virtual bool IsKeyPressed_(int keycode) const = 0;
	virtual bool IsMouseButtonPressed_(int button) const = 0;

	virtual Pos GetMousePosition_() const = 0;
	virtual float GetMouseXPosition_() const = 0;
	virtual float GetMouseYPosition_() const = 0;

private:
	static std::unique_ptr<Input> s_Instatnce;
};