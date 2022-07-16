#pragma once
#include "Core/Inupt.h"

class glfwInput : public Input
{
protected:
	virtual bool IsKeyPressed_(int keycode) const override;
	virtual bool IsMouseButtonPressed_(int button) const override;

	virtual Pos GetMousePosition_() const override;
	virtual float GetMouseXPosition_() const override;
	virtual float GetMouseYPosition_() const override;
};
