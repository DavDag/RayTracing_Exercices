#pragma once

#include "types.hpp"

namespace rt {

	class Ray {
	public:
		explicit Ray(): Ray(Vec3(0.0f), Vec3(0.0f)) { }
		explicit Ray(Vec3 orig, Vec3 dir): orig(orig), dir(dir) { }

		Vec3 operator()(f32 t) const {
			return this->orig + (this->dir * t);
		}

	public:
		Vec3 orig;
		Vec3 dir;
	};

} // namespace rt
