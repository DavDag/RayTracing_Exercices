#pragma once

#include "shape.hpp"

namespace rt {

	class MovingSphere : public Shape {
	public:
		MovingSphere(
			const std::string& name,
			const std::shared_ptr<Material>& mat,
			Vec3 center0, Vec3 center1,
			f32 time0, f32 time1,
			f32 radius
		):
			Shape(name, mat),
			cen0(center0), cen1(center1),
			time0(time0), time1(time1),
			rad(radius)
		{
			//
			_rad2 = rad * rad;
			_cenDelta = cen1 - cen0;
			_timeDelta = time1 - time0;
		}

		void print(std::ostream& out) const override {
			out << "OBJ_MOV_SPHERE " << name << "\n"
				<< "mat = " << mat->name << "\n"
				<< "cen = " << cen0 << " " << cen1 << "\n"
				<< "time = " << time0 << " " << time1 << "\n"
				<< "rad = " << rad << "\n"
				<< "#_rad2 = " << _rad2 << "\n"
				<< "#_cenDelta = " << _cenDelta << "\n"
				<< "#_timeDelta = " << _timeDelta << "\n";
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
			Vec3 cen = _center(ray.time);

			Vec3 oc = ray.orig - cen;
			f32 a = ray.dir.lengthSquared();
			f32 half_b = Vec3::dot(oc, ray.dir);
			f32 c = oc.lengthSquared() - _rad2;
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
			Vec3 norm = Vec3::unit((payload.pos - cen) / rad);
			payload.frontFace = Vec3::dot(ray.dir, norm) < 0;
			payload.norm = (payload.frontFace) ? norm : -norm;
		}

	private:
		Vec3 _center(f32 time) const {
			if (time <= time0) return cen0;
			if (time >= time1) return cen1;
			return Vec3::lerp(cen0, cen1, (time - time0) / _timeDelta);
		}

	private:
		Vec3 cen0, cen1;
		f32 time0, time1;
		f32 rad;
		//
		f32 _rad2;
		Vec3 _cenDelta;
		f32 _timeDelta;
	};

} // namespace rt
