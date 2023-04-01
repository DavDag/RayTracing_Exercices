#pragma once

#include "../core/core.hpp"
#include "surfaceinfo.hpp"

namespace rt {

	class RayHit;

	class Material {
	public:
		Material(const std::string& name) : name(name) { }

		virtual void print(std::ostream& out) const = 0;
		virtual Ray scatter(const Ray& ray, const RayHit& payload, SurfaceInfo& out) const = 0;

	public:
		std::string name;
	};

} // 
