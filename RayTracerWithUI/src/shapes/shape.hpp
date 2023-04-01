#pragma once

#include "../core/core.hpp"
#include "../raytracer/rayhit.hpp"

namespace rt {

	class Material;

	class Shape {
	public:
		Shape(
			const std::string& name,
			const std::shared_ptr<Material>& mat
		):
			name(name), mat(mat)
		{
			//
		}

		virtual void print(std::ostream& out) const = 0;
		virtual void hit(const Ray& ray, RayHit& payload) const = 0;

	public:
		std::string name;
		std::shared_ptr<Material> mat;
	};

} // namespace rt
