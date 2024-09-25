#pragma once

#include "types.hpp"

namespace rt {

	class Ray {
	public:
		explicit Ray(): Ray(Vec3(0.0f), Vec3(0.0f), 0.0f) { }
		explicit Ray(Vec3 orig, Vec3 dir, f32 time): orig(orig), dir(dir), time(time) { }

		Vec3 operator()(f32 t) const {
			return orig + (dir * t);
		}

	public:
		Vec3 orig;
		Vec3 dir;
		f32 time;
	};

} // namespace rt
