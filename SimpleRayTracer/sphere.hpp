#pragma once

#include "vec3.hpp"
#include "ray.hpp"
#include "material.hpp"

class Sphere {
public:
	Sphere(const std::string& name, const Vec3f& cen, Scalar rad, Material* mat): name(name), cen(cen), rad(rad), mat(mat) { }

	bool hit(const Ray& ray, RayHit& hit) const {
		/*
		 * Ray
		 *		R(t) = r_orig + r_dir * t
		 * 
		 * Sphere:
		 *		c_x^2 + c_y^2 + c_z^2 = rad^2
		 * 
		 * Intersection
		 *		(R(t) - c)^2 = rad^2
		 * 
		 * =>   (r_orig + r_dir * t - C)^2 = rad^2
		 * 
		 * =>	(r_orig + r_dir * t - C) * (r_orig + r_dir * t - C) = rad^2
		 * 
		 *		[ro / r_orig, rd / r_dir]
		 * =>	ro^2 + ro*rd*t - ro*C + rd*t*ro + (rd*t)^2 - rd*t*C - C*ro - C*rd*t + C^2 = rad^2
		 *             -------          -------   --------   ------          ------
		 * 
		 *  	[x / t]
		 * =>	ro*rd*x + ro*rd*x + rd^2*x^2 - rd*C*x - rd*C*x + ro^2 - ro*C - ro*C + C^2 = rad^2
		 *      ----------------------------------------------
		 * 
		 * =>   (rd^2)*x^2 + (2*ro*rd-2*rd*C)*x + (ro^2 - 2*ro*C + C^2 - rad^2) = 0
		 * 
		 * =>   (rd^2)*x^2 + (2*rd*(ro-C))*x + ((ro - C)^2 - rad^2) = 0
		 *       ^^^^a        ^^^^^^^^^^^b      ^^^^^^^^^^^^^^^^^^c
		 * 
		 * Solution count (0, 1, 2) represents the number of times the ray intersect the sphere.
		 * 
		 */
		Scalar a = dot(ray.dir, ray.dir); // rd^2
		Scalar b = 2 * dot(ray.dir, (ray.orig - this->cen)); // 2*rd*(ro-C)
		Scalar c = dot(ray.orig - this->cen, ray.orig - this->cen) - (this->rad * this->rad); // (ro - C)^2 - rad^2

		/*
		 * 
		 * delta = b^2 -4*a*c;
		 * x1 = (-b - sqrt(delta)) / 2*a
		 * x2 = (-b + sqrt(delta)) / 2*a
		 * 
		 * x1 always smaller than x2
		 * 
		 */
		Scalar delta = b * b - 4 * a * c;
		if (delta < 0)
			return false;
		Scalar x1 = (-b - std::sqrt(delta)) / (2.0f * a);
		Scalar x2 = (-b + std::sqrt(delta)) / (2.0f * a);

		Scalar res = x1;
		if (res < hit.tmin || res > hit.tmax)
			res = x2;
		if (res < hit.tmin || res > hit.tmax)
			return false;

		/*
		 * Hit data
		 */
		hit.t = res;
		hit.p = ray(res);
		//
		Vec3f norm = (hit.p - this->cen) / this->rad;
		hit.frontFace = dot(ray.dir, norm) < 0;
		hit.nor = (hit.frontFace) ? norm : -norm;
		//
		hit.mat = this->mat;
		return true;
	}

public:
	std::string name;
	Vec3f cen;
	Scalar rad;
	Material* mat;
};
