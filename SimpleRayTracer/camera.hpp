#pragma once

#include "vec3.hpp"
#include "ray.hpp"

class Camera {
public:
	Camera(const Vec3f& pos, int vw, int vh): pos(pos), vw(vw), vh(vh) {
		this->aspect = vw / (float)vh;
	}

	Ray getRayFor(float px, float py) const {
		float x = (px - this->vw / 2.0f) / this->vw;
		float y = (py - this->vh / 2.0f) / this->vh;
		Vec3f orig = this->pos;
		Vec3f dir = Vec3f(x * this->aspect, -y, 0) - orig;
		dir.normalize();
		return Ray(orig, dir);
	}

public:
	Vec3f pos;
	int vw, vh;
	float aspect;
};
