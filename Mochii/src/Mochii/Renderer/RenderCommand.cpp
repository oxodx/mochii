#include "RenderCommand.h"
#include "Mochii/Platform/OpenGL/OpenGLRendererAPI.h"
#include "mzpch.h"

namespace Mochii {
Scope<RendererAPI> RenderCommand::_RendererAPI =
    CreateScope<OpenGLRendererAPI>();
}
