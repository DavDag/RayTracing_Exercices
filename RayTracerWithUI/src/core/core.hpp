#pragma once

#include "types.hpp"
#include "crc32.hpp"
#include "pixel.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "rnd.hpp"

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <sstream>
#include <fstream>
#include <iostream>

namespace rt {

	static constexpr f32 PI = 3.141592f;

	static constexpr f32 degToRad(f32 degrees) {
		return degrees * PI / 180.0f;
	}

} // namespace rt
