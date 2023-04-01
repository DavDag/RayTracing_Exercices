#pragma once

#include "material.hpp"

namespace rt {

	class Diffusive: public Material {
	public:
		Diffusive(const std::string& name) : Material(name) { }

	private:
		Vec3 albedo;
	};

} // namespace rt
