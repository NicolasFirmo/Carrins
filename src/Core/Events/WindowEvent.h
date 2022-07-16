#pragma once
#include "Event.h"

struct CloseEvent : public Event
{
};

struct ResizeEvent : public Event
{
	const int Width, Height;

	ResizeEvent(int width, int height) : Width(width), Height(height) {}
};
