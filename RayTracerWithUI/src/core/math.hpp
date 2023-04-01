#pragma once

#include "types.hpp"

namespace rt {

	static constexpr f32 PI = 3.141592f;

	static constexpr f32 degToRad(f32 degrees) {
		return degrees * PI / 180.0f;
	}

} // namespace rt

