#pragma once

#include "types.hpp"

#include <random>

namespace rt {

	template<typename T>
	T rnd_uniform(T min, T max) {
		static thread_local std::uniform_real_distribution<f32> distribution(0.0f, 0.0f);
		static thread_local std::mt19937 generator;
		return min + T((max - min) * distribution(generator));
	}

	template<typename T>
	T rnd_normal(T min, T max) {
		static thread_local std::normal_distribution<f32> distribution(0.0f, 1.0f);
		static thread_local std::mt19937 generator;
		return min + T((max - min) * distribution(generator));
	}

} // namespace rt

