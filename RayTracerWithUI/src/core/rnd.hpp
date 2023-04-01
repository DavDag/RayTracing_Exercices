#pragma once

#include "types.hpp"

#include <random>

namespace rt {

	template<typename T>
	T rndT(T min = 0.0f, T max = 1.0f) {
		static thread_local std::uniform_real_distribution<T> distribution(0.0f, 1.0f);
		static thread_local std::mt19937 generator;
		return min + (max - min) * distribution(generator);
	}

} // namespace rt

