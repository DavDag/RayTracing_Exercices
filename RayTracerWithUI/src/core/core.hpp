#pragma once

#include "types.hpp"
#include "crc32.hpp"
#include "pixel.hpp"
#include "vec3.hpp"
#include "ray.hpp"

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <sstream>
#include <fstream>
#include <iostream>

static constexpr f32 pi = 3.141592f;

static constexpr f32 degreesToRadians(f32 degrees) {
	return degrees * pi / 180.0f;
}
