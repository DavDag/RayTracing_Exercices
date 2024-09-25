#pragma once

#include "material.hpp"

namespace rt {

	class Dielectric : public Material {
	public:
		Dielectric(
			const std::string& name,
			f32 refractionRatio
		) :
			Material(name), _refractionRatio(refractionRatio)
		{
			_refractionRatioInv = 1.0f / refractionRatio;
		}

		void print(std::ostream& out) const override {
			out << "MAT_DIEL " << name << "\n"
				<< "refractionRatio = " << _refractionRatio << "\n";
		}

		bool scatter(
			const Ray& ray, const RayHit& payload,
			SurfaceInfo& out, Ray& scattered
		) const override {
			f32 rr = (payload.frontFace) ? _refractionRatioInv : _refractionRatio;
			f32 cosTheta = std::min<f32>(Vec3::dot(-ray.dir, payload.norm), 1.0f);
			f32 sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);
			bool cannotRefract = rr * sinTheta > 1.0f;

			Vec3 scatteredDir(0.0f);
			if (cannotRefract || reflectance(cosTheta, rr) > rnd_uniform<f32>(0.0f, 1.0f)) {
				scatteredDir = Vec3::reflect(ray.dir, payload.norm);
			}
			else {
				Vec3 perp = (ray.dir + payload.norm * cosTheta) * rr;
				Vec3 para = payload.norm * -std::sqrt(std::abs(1.0f - perp.lengthSquared()));
				scatteredDir = perp + para;
			}

			out.attenuation = Color(1.0f);
			scattered = Ray(payload.pos, Vec3::unit(scatteredDir), ray.time);
			return true;
		}

	private:
		static f32 reflectance(f32 cosine, f32 ref_idx) {
			// Use Schlick's approximation for reflectance.
			f32 r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
			r0 = r0 * r0;
			return r0 + (1.0f - r0) * std::powf((1 - cosine), 5);
		}

	private:
		f32 _refractionRatio;
		f32 _refractionRatioInv;
	};

} // namespace rt
