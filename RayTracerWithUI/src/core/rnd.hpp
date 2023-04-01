#pragma once

#include "types.hpp"

#include <random>
#include <chrono>

namespace rt {

	template<typename T>
	T rnd_uniform(T min, T max) {
		static thread_local auto seed = std::chrono::system_clock::now().time_since_epoch().count();
		static thread_local std::uniform_real_distribution<f32> distribution(0.0f, 1.0f);
		static thread_local std::mt19937 generator(seed);
		return min + T((max - min) * distribution(generator));
	}

	template<typename T>
	T rnd_normal(T min, T max) {
		static thread_local auto seed = std::chrono::system_clock::now().time_since_epoch().count();
		static thread_local std::normal_distribution<f32> distribution(0.0f, 1.0f);
		static thread_local std::mt19937 generator(seed);
		return min + T((max - min) * distribution(generator));
	}

} // namespace rt

