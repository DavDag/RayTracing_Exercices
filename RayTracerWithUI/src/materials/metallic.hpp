#pragma once

#include "material.hpp"

namespace rt {

	class Metallic : public Material {
	public:
		Metallic(
			const std::string& name,
			Color albedo, f32 fuzziness
		) :
			Material(name), _albedo(albedo), _fuzziness(fuzziness)
		{
			//
		}

		void print(std::ostream& out) const override {
			out << "MAT_METAL " << name << "\n"
				<< "albedo = " << _albedo << "\n"
				<< "fuzziness = " << _fuzziness << "\n";
		}

		bool scatter(
			const Ray& ray, const RayHit& payload,
			SurfaceInfo& out, Ray& scattered
		) const override {
			Vec3 reflected = Vec3::reflect(ray.dir, payload.norm);
			Vec3 scatteredDir = reflected + Vec3::rndInUnitSphere() * _fuzziness;
			out.attenuation = _albedo;
			scattered = Ray(payload.pos, Vec3::unit(scatteredDir));
			return Vec3::dot(scattered.dir, payload.norm) > 0;
		}

	private:
		Color _albedo;
		f32 _fuzziness;
	};

} // namespace rt
