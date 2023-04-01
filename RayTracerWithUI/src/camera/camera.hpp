#pragma once

#include "../core/core.hpp"

namespace rt {

	class Camera {
	public:
		Camera(Vec3 pos, Vec3 target, f32 aspect, f32 fovy);

		Ray getRay(f32 dx, f32 dy) const;

	private:
		Vec3 _pos, _target;
		f32 _aspect, _fovy;
		Vec3 _xInc, _yInc;
	};

} // namespace rt
