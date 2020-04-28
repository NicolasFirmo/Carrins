#include "OpenGLContext.h"
#include "OpenGLCore.h"

#include "Instrumentation/Profile.h"

void OpenGLContext::Init(void *procAddress)
{
	NIC_PROFILE_FUNCTION();

	NIC_ASSERT(gladLoadGLLoader((GLADloadproc)procAddress), "Glad could not load OpenGl");

	DebugLog("OpenGL info:\n");
	DebugLog("	Vendor: " << glGetString(GL_VENDOR) << '\n');
	DebugLog("	Renderer: " << glGetString(GL_RENDERER) << '\n');
	DebugLog("	Version: " << glGetString(GL_VERSION) << '\n');
}