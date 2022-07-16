#pragma once
#include "Event.h"

struct KeyEvent : public Event
{
	enum class T
	{
		Pressed,
		Repeated,
		Released,
	} const Type;
	const int Code;

	KeyEvent(T type, int code) : Type(type), Code(code) {}
};

struct CharEvent : public Event
{
	const unsigned Code;

	CharEvent(unsigned code) : Code(code) {}
};
