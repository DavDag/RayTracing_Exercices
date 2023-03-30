#pragma once

#include <string>
#include <fstream>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////

struct Scene {
	std::string sceneFile{};
	friend static std::ostream& operator <<(std::ostream& out, const Scene& scene);
};

struct Pixel {
	float r = 0.f;
	float g = 0.f;
	float b = 0.f;
};

Pixel* process(const std::string& scene, int w, int h);
uint8_t* convertToRawPixels(const Pixel* pixels, int w, int h);

////////////////////////////////////////////////////////////////////////////////////
