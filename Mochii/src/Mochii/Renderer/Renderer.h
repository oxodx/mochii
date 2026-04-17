#pragma once

namespace Mochii {
	enum class RendererAPI {
		None = 0, OpenGL = 1
	};

	class Renderer {
	public:
		inline static RendererAPI GetAPI() { return _RendererAPI; }
	private:
		static RendererAPI _RendererAPI;
	};
}
