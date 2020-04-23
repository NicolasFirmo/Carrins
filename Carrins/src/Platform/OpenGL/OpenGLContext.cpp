#include "OpenGLContext.h"
#include <glad/glad.h>

#include "Core.h"

void OpenGLContext::Init(void *procAddress)
{
	NIC_ASSERT(gladLoadGLLoader((GLADloadproc)procAddress), "Glad could not load OpenGl");

	DebugLog("OpenGL info:\n");
	DebugLog("	Vendor: " << glGetString(GL_VENDOR) << '\n');
	DebugLog("	Renderer: " << glGetString(GL_RENDERER) << '\n');
	DebugLog("	Version: " << glGetString(GL_VERSION) << '\n');
}