#pragma once

#include "../core/core.hpp"
#include "../materials/material.hpp"

namespace rt {

	class Shape {
	public:
		Shape(const std::string& name, const std::shared_ptr<Material>& mat): name(name), mat(mat) { }

	public:
		std::string name;
		std::shared_ptr<Material> mat;
	};

} // namespace rt
