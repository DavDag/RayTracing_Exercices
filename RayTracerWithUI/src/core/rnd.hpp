#pragma once

#include "types.hpp"

#include <random>
#include <chrono>

namespace rt {

	template<typename T>
	T rnd_uniform(T min, T max) {
		static thread_local std::uniform_real_distribution<f32> distribution(0.0f, 1.0f);
		static thread_local std::minstd_rand generator(std::random_device{}());
		return min + T((max - min) * distribution(generator));
	}

	template<typename T>
	T rnd_normal(T min, T max) {
		static thread_local std::normal_distribution<f32> distribution(0.0f, 1.0f);
		static thread_local std::minstd_rand generator(std::random_device{}());
		return min + T((max - min) * distribution(generator));
	}

} // namespace rt

