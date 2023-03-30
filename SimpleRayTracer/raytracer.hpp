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

Scene parseScene(const std::string& scenefile);
Vec3f* process(const Scene& scene, int w, int h);
Vec3f processPixel(const Scene& scene, int px, int py);
Vec3f traceRay(const Scene& scene, const Ray& ray, int depth);
