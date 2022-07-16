#pragma once
#include "Event.h"

struct MouseMoveEvent : public Event
{
	const float PosX, PosY;

	MouseMoveEvent(float posX, float posY) : PosX(posX), PosY(posY) {}
};

struct MouseButtonEvent : public Event
{
	enum class T
	{
		Pressed,
		Released,
	} const Type;
	const int Button;

	MouseButtonEvent(T type, int button) : Type(type), Button(button) {}
};

struct MouseScrollEvent : public Event
{
	const float DeltaX, DeltaY;

	MouseScrollEvent(float deltaX, float deltaY) : DeltaX(deltaX), DeltaY(deltaY) {}
};