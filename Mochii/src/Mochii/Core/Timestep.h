#pragma once

namespace Mochii {
	class Timestep {
	public:
		Timestep(float time = 0.0f)
			: _Time(time) { }

		operator float() const { return _Time; }

		float GetSeconds() const { return _Time; }
		float GetMilliseconds() const { return _Time * 1000.0f; }
	private:
		float _Time;
	};
}
