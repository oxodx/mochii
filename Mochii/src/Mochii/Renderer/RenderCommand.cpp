#include "mzpch.h"
#include "RenderCommand.h"
#include "Mochii/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Mochii {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}
