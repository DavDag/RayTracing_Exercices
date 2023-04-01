#pragma once

#include "../core/core.hpp"

namespace rt {

	class Shape;

	class RayHit {
	public:
		RayHit():
			obj(nullptr), tmin(0.0001f), tmax(f32max),
			frontFace(false), pos(0.0f), norm(0.0f)
		{
			//
		}

		operator bool() const {
			return (obj != nullptr);
		}

	public:
		const Shape* obj;
		f32 tmin, tmax;
		bool frontFace;
		Vec3 pos, norm;
	};

} // rt
