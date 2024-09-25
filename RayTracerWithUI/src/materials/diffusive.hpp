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
			out << "MAT_DIFF " << name << "\n"
				<< "albedo = " << albedo << "\n";
		}

		bool scatter(
			const Ray& ray, const RayHit& payload,
			SurfaceInfo& out, Ray& scattered
		) const override {
			Vec3 scatteredDir = payload.norm + Vec3::rndUnitVector();
			if (scatteredDir.isZero())
				scatteredDir = payload.norm;
			out.attenuation = albedo;
			scattered = Ray(payload.pos, Vec3::unit(scatteredDir), ray.time);
			return true;
		}

	private:
		Color albedo;
	};

} // namespace rt
