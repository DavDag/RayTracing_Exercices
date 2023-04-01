#pragma once

#include "../core/core.hpp"

namespace rt {

	class SurfaceInfo {
	public:
		SurfaceInfo(): attenuation(0.0f) { }

	public:
		Color attenuation;
	};

} // namespace rt
