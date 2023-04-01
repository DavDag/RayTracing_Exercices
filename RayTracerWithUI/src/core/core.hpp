#pragma once

#ifdef _DEBUG
#define DBG_ASSERT(eq) do { if (!eq) { __debugbreak(); exit(1); } } while(0);
#else
#define DBG_ASSERT(eq) 
#endif // DEBUG

#include "types.hpp"
#include "math.hpp"
#include "crc32.hpp"
#include "color.hpp"
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

