#pragma once

#include "vec3.hpp"
#include "ray.hpp"
#include "rnd.hpp"

class Material {
public:
	virtual bool scatter(const Ray& ray, RayHit& hit, Vec3f& attenuation, Ray& out) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(const Vec3f& albedo) : Material(), albedo(albedo) { }

	bool scatter(const Ray& ray, RayHit& hit, Vec3f& attenuation, Ray& out) const override {
		Vec3f scatterDir = hit.nor + RandomVec3f(-1.0f, 1.0f).normalized();
		if (IsZero(scatterDir)) {
			scatterDir = hit.nor;
		}
		else {
			scatterDir.normalize();
		}
		out = Ray(hit.p, scatterDir);
		attenuation = albedo;
		return true;
	}

public:
	Vec3f albedo;
};

class Metal : public Material {
public:
	Metal(const Vec3f& albedo, Scalar blurr): Material(), albedo(albedo), blurr(std::min(blurr, (Scalar)1.0)) { }

	bool scatter(const Ray& ray, RayHit& hit, Vec3f& attenuation, Ray& out) const override {
		Vec3f reflected = ray.dir.reflect(hit.nor).normalized();
		Vec3f scattered = reflected + this->blurr * RandomVec3f(-1.0f, 1.0f);
		scattered.normalize();
		out = Ray(hit.p, scattered);
		attenuation = albedo;
		return (dot(out.dir, hit.nor) > 0);
	}

public:
	Vec3f albedo;
	Scalar blurr;
};

class Dielectric : public Material {
public:
	Dielectric(Scalar refractionRatio) : Material(), refractionRatio(refractionRatio) { }

	bool scatter(const Ray& ray, RayHit& hit, Vec3f& attenuation, Ray& out) const override {
		attenuation = Vec3f(1.0f);
		Scalar rr = (hit.frontFace) ? Scalar(1.0) / this->refractionRatio : this->refractionRatio;

		Scalar cosTheta = std::min(dot(-ray.dir, hit.nor), Scalar(1.0));
		Scalar sinTheta = std::sqrt(Scalar(1.0) - cosTheta * cosTheta);
		bool cannotRefract = rr * sinTheta > 1.0;

		Vec3f scatterDir(0.0f);
		if (cannotRefract || reflectance(cosTheta, rr) > rndDouble()) {
			scatterDir = ray.dir.reflect(hit.nor);
		}
		else {
			Vec3f perp = rr * (ray.dir + cosTheta * hit.nor);
			Vec3f para = -std::sqrt(std::abs(Scalar(1.0) - perp.lengthSquared())) * hit.nor;
			scatterDir = perp + para;
		}

		out = Ray(hit.p, scatterDir);
		return true;
	}

public:
	Scalar refractionRatio;
	
private:
	static Scalar reflectance(Scalar cosine, Scalar ref_idx) {
		// Use Schlick's approximation for reflectance.
		Scalar r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};
