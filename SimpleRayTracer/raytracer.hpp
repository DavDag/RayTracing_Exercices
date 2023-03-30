#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "vec3.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include "sphere.hpp"

struct Scene {
	std::string sceneFile{};
	Camera camera;
	std::vector<Sphere*> spheres;
	std::vector<Material*> materials;
};

static std::ostream& operator <<(std::ostream& out, const Scene& scene) {
	out << "[Scene]:\n"
		<< "Scene file: " << scene.sceneFile << "\n";
	return out;
}

Scene parseScene(const std::string& scenefile, int w, int h);
Vec3f* process(const Scene& scene, int w, int h, int samples, int depthMax);
Vec3f processPixel(const Scene& scene, int px, int py, int samples, int depthMax);
Vec3f traceRay(const Scene& scene, const Ray& ray, int depth);
