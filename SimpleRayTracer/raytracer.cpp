#include "raytracer.hpp"

#include "rnd.hpp"
#include <limits>

#include <ppl.h>

Scene parseScene(const std::string& scenefile) {
	std::cout << "\n[Parsing]\n";

	std::ifstream in(scenefile);
	if (!in.is_open()) {
		std::cerr << "Unable to parse scene file\n";
		exit(EXIT_FAILURE);
	}

	// 1. Camera
	Vec3f cameraPos(0);
	int cameraVW = 0, cameraVH = 0;
	in >> cameraPos.x >> cameraPos.y >> cameraPos.z;
	in >> cameraVW >> cameraVH;

	// 2. Materials
	int materialCount = 0;
	std::vector<Material*> materials;
	in >> materialCount;
	for (int i = 0; i < materialCount; ++i) {
		Vec3f albedo;
		in >> albedo.x >> albedo.y >> albedo.z;
		materials.push_back(new Lambertian(albedo));
	}
	in >> materialCount;
	for (int i = 0; i < materialCount; ++i) {
		Vec3f albedo;
		Scalar blurr;
		in >> albedo.x >> albedo.y >> albedo.z >> blurr;
		materials.push_back(new Metal(albedo, blurr));
	}
	in >> materialCount;
	for (int i = 0; i < materialCount; ++i) {
		Scalar refractionRatio;
		in >> refractionRatio;
		materials.push_back(new Dielectric(refractionRatio));
	}

	// 3. Spheres
	int sphereCount = 0;
	in >> sphereCount;
	std::vector<Sphere*> spheres;
	for (int i = 0; i < sphereCount; ++i) {
		Vec3f cen;
		Scalar rad;
		int materialIndex;
		in >> cen.x >> cen.y >> cen.z >> rad >> materialIndex;
		spheres.push_back(new Sphere(cen, rad, materials[materialIndex]));
	}

	//
	return Scene{
		.sceneFile = scenefile,
		.camera = Camera(cameraPos, cameraVW, cameraVH),
		.spheres = spheres,
	};
}

Vec3f* process(const Scene& scene, int w, int h) {
	std::cout << "\n[Processing]\n";

	Vec3f* pixels = new Vec3f[(size_t) w * h];
	memset(pixels, 0, sizeof(Vec3f) * w * h);

	auto part = concurrency::affinity_partitioner();
	concurrency::parallel_for(0, w, [&scene, &w, &h, &pixels](size_t x) {
		for (int y = 0; y < h; ++y) {
			Vec3f& p = pixels[y * w + x];
			p = processPixel(scene, x, y);
			p = gamma(p, 2.0);
		}
	}, part);

	return pixels;
}

Vec3f processPixel(const Scene& scene, int px, int py) {
	constexpr int samples = 128;
	constexpr int depthMax = 64;
	Vec3f out;
	for (int i = 0; i < samples; ++i) {
		Ray ray = scene.camera.getRayFor(px + rndFloat(), py + rndFloat());
		out += traceRay(scene, ray, depthMax);
	}
	return out / samples;
}

Vec3f traceRay(const Scene& scene, const Ray& ray, int depth) {
	if (depth == 0)
		return Vec3f(0.0f);

	RayHit hit;
	hit.tmin = 0.001;
	hit.tmax = std::numeric_limits<Scalar>::max();

	for (const auto& sphere : scene.spheres)
		if (sphere->hit(ray, hit))
			hit.tmax = hit.t;

	if (hit.mat != nullptr) {
		Ray scattered{};
		Vec3f attenuation(0.0f);
		if (hit.mat->scatter(ray, hit, attenuation, scattered)) {
			Vec3f res = traceRay(scene, scattered, depth - 1);
			return Vec3f(res.x * attenuation.x, res.y * attenuation.y, res.z * attenuation.z);
		}
		else {
			return Vec3f(0.0f);
		}
	}
	else {
		// Sky (athmosphere)
		static Vec3f SkyColorA = Vec3f(1.0f, 1.0f, 1.0f);
		static Vec3f SkyColorB = Vec3f(0.5f, 0.7f, 1.0f);
		Scalar t = ray.dir.normalized().y / 2.0f + 0.5f;
		return lerp(SkyColorA, SkyColorB, t);
	}
}
