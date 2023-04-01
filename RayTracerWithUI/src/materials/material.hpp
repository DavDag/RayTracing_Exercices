#pragma once

#include "../core/core.hpp"

namespace rt {

	class Material {
	public:
		Material(const std::string& name): name(name) { }

	public:
		std::string name;
	};

} // 
