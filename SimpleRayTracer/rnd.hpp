#pragma once

#include <random>

static float rndFloat(float min = 0.0f, float max = 1.0f) {
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static std::mt19937 generator;
	return min + (max - min) * distribution(generator);
}

static double rndDouble(double min = 0.0f, double max = 1.0f) {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return min + (max - min) * distribution(generator);
}
