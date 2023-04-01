#pragma once

#include "shape.hpp"

namespace rt {

	class Sphere : public Shape {
	public:
		Sphere(
			const std::string& name,
			const std::shared_ptr<Material>& mat,
			Vec3 center, f32 radius
		):
			Shape(name, mat), cen(center), rad(radius)
		{
			//
		}

		void print(std::ostream& out) const override {
			out << "OBJ_SPHERE " << this->name << "\n"
				<< "mat = " << this->mat->name << "\n"
				<< "cen = " << this->cen << "\n"
				<< "rad = " << this->rad << "\n";
		}

		void hit(const Ray& ray, RayHit& payload) const override {
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

			// rd^2
			f32 a = Vec3::dot(ray.dir, ray.dir);

			// 2*rd*(ro-C)
			f32 b = 2.0f * Vec3::dot(ray.dir, (ray.orig - this->cen));

			// (ro-C)^2 - rad^2
			f32 c = Vec3::dot(ray.orig - this->cen, ray.orig - this->cen) - (this->rad * this->rad);

			/*
			 * 
			 * delta = b^2 - 4*a*c;
			 * x1 = (-b - sqrt(delta)) / 2*a
			 * x2 = (-b + sqrt(delta)) / 2*a
			 * 
			 * x1 always smaller than x2
			 * 
			 */
			f32 delta = b * b - 4 * a * c;
			if (delta < 0) return;

			f32 x1 = (-b - std::sqrt(delta)) / (2.0f * a);
			f32 x2 = (-b + std::sqrt(delta)) / (2.0f * a);

			f32 res = x1;
			if (res < payload.tmin || res > payload.tmax) res = x2;
			if (res < payload.tmin || res > payload.tmax) return;

			// Load payload
			payload.obj = this;
			payload.tmax = res;
			payload.pos = ray(res);
			Vec3 norm = (payload.pos - this->cen) / this->rad;
			payload.frontFace = Vec3::dot(ray.dir, norm) < 0;
			payload.norm = (payload.frontFace) ? norm : -norm;
		}

	private:
		Vec3 cen;
		f32 rad;
	};

} // namespace rt
