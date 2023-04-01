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
			out << "MAT_METAL " << this->name << "\n"
				<< "albedo = " << this->_albedo << "\n"
				<< "fuzziness = " << this->_fuzziness << "\n";
		}

		Ray scatter(const Ray& ray, const RayHit& payload, SurfaceInfo& out) const override {
			Vec3 reflected = Vec3::reflect(ray.dir, payload.norm);
			Vec3 scattered = reflected + (Vec3::rndInUnitSphere() * this->_fuzziness);
			out.attenuation = this->_albedo;
			return Ray(payload.pos, scattered);
		}

	private:
		Color _albedo;
		f32 _fuzziness;
	};

} // namespace rt
