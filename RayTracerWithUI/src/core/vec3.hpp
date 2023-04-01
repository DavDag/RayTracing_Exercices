#pragma once

#include "types.hpp"

namespace rt {

	class Vec3 {
		using T = f32;

	public:
		explicit Vec3(): Vec3(0.0f) { }
		explicit Vec3(T v): Vec3(v, v, v) { }
		explicit Vec3(T x, T y, T z): x(x), y(y), z(z) { }



	public:
		T x, y, z;
	};

} // namespace rt
