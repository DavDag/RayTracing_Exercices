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
			out << "OBJ_SPHERE " << name << "\n"
				<< "mat = " << mat->name << "\n"
				<< "cen = " << cen << "\n"
				<< "rad = " << rad << "\n";
		}

		void hit(const Ray& ray, RayHit& payload) const override {
			/*
			f32 a = Vec3::dot(ray.dir, ray.dir);
			f32 b = 2.0f * Vec3::dot(ray.dir, (ray.orig - cen));
			f32 c = Vec3::dot(ray.orig - cen, ray.orig - cen) - (rad * rad);

			f32 delta = b * b - 4 * a * c;
			if (delta < 0) return;

			f32 x1 = (-b - std::sqrt(delta)) / (2.0f * a);
			f32 x2 = (-b + std::sqrt(delta)) / (2.0f * a);

			f32 res = x1;
			if (res < payload.tmin || res > payload.tmax) res = x2;
			if (res < payload.tmin || res > payload.tmax) return;
			*/

			Vec3 oc = ray.orig - cen;
			f32 a = ray.dir.lengthSquared();
			f32 half_b = Vec3::dot(oc, ray.dir);
			f32 c = oc.lengthSquared() - rad * rad;
			f32 delta = half_b * half_b - a * c;
			if (delta < 0) return;

			f32 sqrtd = sqrt(delta);
			f32 res = (-half_b - sqrtd) / a;
			if (res < payload.tmin || res > payload.tmax) res = (-half_b + sqrtd) / a;
			if (res < payload.tmin || res > payload.tmax) return;

			// Load payload
			payload.obj = this;
			payload.tmax = res;
			payload.pos = ray(res);
			Vec3 norm = (payload.pos - cen) / rad;
			payload.frontFace = Vec3::dot(ray.dir, norm) < 0;
			payload.norm = (payload.frontFace) ? norm : -norm;
		}

	private:
		Vec3 cen;
		f32 rad;
	};

} // namespace rt
