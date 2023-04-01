#pragma once

#include "../core/core.hpp"

namespace rt {

	class Camera {
	public:
		Camera(Vec3 pos, Vec3 target, f32 fovy);

	private:
		Vec3 _pos, _target;
		f32 _fovy;
	};

} // namespace rt
