#pragma once

#include "../core/core.hpp"

namespace rt {

	class Camera {
	public:
		Camera(Vec3 pos, Vec3 target, i32 imgw, i32 imgh, f32 fovy);

		void print(std::ostream& out) const;

		Ray getRay(f32 dx, f32 dy) const;
		i32 imgW() const;
		i32 imgH() const;

	private:
		Vec3 _pos, _target;
		i32 _imgw, _imgh;
		f32 _fovy;
		Vec3 _xInc, _yInc;
		f32 _aspect;
	};

} // namespace rt
