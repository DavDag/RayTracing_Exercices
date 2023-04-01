#pragma once

#include "../core/core.hpp"

namespace rt {

	class Shape;

	class RayHit {
	public:
		RayHit(): obj(nullptr), tmin(0.0001f), tmax(f32max), pos(0.0f), norm(0.0f) { }

		operator bool() const {
			return (this->obj != nullptr);
		}

	public:
		const Shape* obj;
		f32 tmin, tmax;
		Vec3 pos, norm;
	};

} // rt
