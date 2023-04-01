#pragma once

#include "material.hpp"

namespace rt {

	class Diffusive: public Material {
	public:
		Diffusive(
			const std::string& name,
			Color albedo
		) :
			Material(name), albedo(albedo)
		{
			//
		}

		void print(std::ostream& out) const override {
			out << "MAT_DIFF " << this->name << "\n"
				<< "albedo = " << this->albedo << "\n";
		}

		Ray scatter(const Ray& ray, const RayHit& payload, SurfaceInfo& out) const override {
			Vec3 scattered = payload.norm + Vec3::rndInUnitSphere();
			//if (scattered.isZero())
			//	scattered = payload.norm;
			out.attenuation = this->albedo;
			return Ray(payload.pos, Vec3::unit(scattered));
		}

	private:
		Color albedo;
	};

} // namespace rt
