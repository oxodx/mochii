#include "mzpch.h"
#include "RenderCommand.h"
#include "Mochii/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Mochii {
	Scope<RendererAPI> RenderCommand::_RendererAPI = CreateScope<OpenGLRendererAPI>();
}
