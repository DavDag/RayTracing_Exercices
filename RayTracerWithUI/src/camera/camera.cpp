#include "camera.hpp"

namespace rt {

	Camera::Camera(Vec3 pos, Vec3 target, f32 aspect, f32 fovy) :
		_pos(pos), _target(target), _aspect(aspect), _fovy(fovy)
	{
		// Viewport using aspect ration + fovy
		f32 theta = degToRad(fovy);
		f32 viewh = 2.0f * std::tan(theta / 2.0f);
		f32 vieww = this->_aspect * viewh;

		// Lookat
		Vec3 w = Vec3::unit(this->_pos - this->_target);
		Vec3 u = Vec3::cross(Vec3(0, 1, 0), w);
		Vec3 v = Vec3::cross(w, u);

		// Increments per-axis
		this->_xInc = u * vieww;
		this->_yInc = v * viewh;
	}

	Ray Camera::getRay(f32 dx, f32 dy) const {
		// [0,1] -> [-1,1]
		f32 x = dx * 2.0f - 1.0f;
		f32 y = dy * 2.0f - 1.0f;

		// Project using increment pex-axis
		Vec3 xinc = this->_xInc * x;
		Vec3 yinc = this->_yInc * y;

		// Create ray
		Vec3 rayOrig = this->_pos;
		Vec3 rayDir = xinc + (-yinc) - this->_pos;

		return Ray(rayOrig, Vec3::unit(rayDir));
	}

} // namespace rt
