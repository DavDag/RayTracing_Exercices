#pragma once

#include "shape.hpp"

namespace rt {

	class Sphere : public Shape {
	public:
		Sphere(const std::string& name, const std::shared_ptr<Material>& mat): Shape(name, mat) { }

	private:
		Vec3 cen;
		f32 rad;
	};

} // namespace rt
