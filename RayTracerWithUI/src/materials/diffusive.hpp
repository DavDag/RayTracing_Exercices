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
			out << "[" << this->name << "]\n"
				<< "albedo = " << this->albedo << "\n";
		}

		Ray scatter(const Ray& ray, const RayHit& payload, SurfaceInfo& out) const override {
			Vec3 scatterDir = payload.norm + Vec3::rndInUnitSphere();
			if (scatterDir.isZero()) {
				scatterDir = payload.norm;
			}
			else {
				scatterDir = Vec3::unit(scatterDir);
			}
			out.attenuation = this->albedo;
			return Ray(payload.pos, scatterDir);
		}

	private:
		Color albedo;
	};

} // namespace rt
