#pragma once

#include "vec3.hpp"

class Material;

class Ray {
public:
	Ray(): Ray(Vec3f(0), Vec3f(0)) { }
	Ray(const Vec3f& orig, const Vec3f& dir): orig(orig), dir(dir) { }

	Vec3f operator()(Scalar t) const {
		return orig + dir * t;
	}

public:
	Vec3f orig;
	Vec3f dir;
};

struct RayHit {
	Vec3f p{};					// where (world-space)
	Scalar t{};					// when (ray equation)
	Scalar tmin{}, tmax{};		// valid range
	bool frontFace = false;		// wheter the ray hit a surface from the front face
	Vec3f nor{};				// normal at collision point
	Material* mat = nullptr;	// material of collided shape
};
