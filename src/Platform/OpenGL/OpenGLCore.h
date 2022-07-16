#pragma once
#include <glad/glad.h>
#include "Core.h"

#ifndef NDEBUG
	#define GLCall(x) while(glGetError()); x; while(auto error = glGetError())NIC_ASSERT(false, std::hex << "[OpenGL error] (0x0" << error << std::dec << "): " << #x)
#else
	#define GLCall(x) x
#endif