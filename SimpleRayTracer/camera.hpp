#pragma once

#include "vec3.hpp"
#include "ray.hpp"

class Camera {
public:
	Camera(const Vec3f& pos, const Vec3f& target, int imgw, int imgh, float fovy):
		pos(pos), target(target), imgw(imgw), imgh(imgh), fovy(fovy)
	{
		this->aspect = imgw / (float)imgh;
		constexpr float pi = 3.141592f;
		float theta = fovy * pi / 180.0f;
		float viewh = 2.0f * std::tan(theta / 2.0f);
		float vieww = this->aspect * viewh;

		Vec3f w = this->pos - this->target;
		w.normalize();
		Vec3f u = cross(Vec3f(0, 1, 0), w);
		Vec3f v = cross(w, u);

		this->vw = vieww * u;
		this->vh = viewh * v;
	}

	Ray getRayFor(float px, float py) const {
		float nx = (px / (float)this->imgw) * 2.0f - 1.0f;
		float ny = (py / (float)this->imgh) * 2.0f - 1.0f;
		Vec3f x = nx * this->vw;
		Vec3f y = ny * this->vh;
		Vec3f orig = this->pos;
		Vec3f dir = x + (-y) -orig;
		dir.normalize();
		return Ray(orig, dir);
	}

public:
	Vec3f pos;
	Vec3f target;
	int imgw, imgh;
	float fovy;
	Vec3f vw, vh;
	float aspect;
};
