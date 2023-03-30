#include "raytracer.hpp"

static std::ostream& operator <<(std::ostream& out, const Scene& scene) {
	out << "[Scene]:\n"
		<< "Scene file: " << scene.sceneFile << "\n";
	return out;
}

Pixel* process(const std::string& scene, int w, int h) {
	std::cout << "\n[Processing]\n";
	Pixel* pixels = new Pixel[w * h];
	memset(pixels, 0, sizeof(Pixel) * w * h);
	for (int y = 0; y < h; ++y)
		for (int x = 0; x < w; ++x) {
			Pixel& p = pixels[y * w + x];
			p.r = (y / (float)h);
			p.g = (x / (float)w);
		}
	return pixels;
}

uint8_t* convertToRawPixels(const Pixel* pixels, int w, int h) {
	std::cout << "\n[Converting]\n";
	uint8_t* raw = new uint8_t[3 * w * h];
	for (int y = 0; y < h; ++y)
		for (int x = 0; x < w; ++x) {
			const Pixel& p = pixels[y * w + x];
			raw[(y * w + x) * 3 + 0] = int(p.r * 255) & 0xff;
			raw[(y * w + x) * 3 + 1] = int(p.g * 255) & 0xff;
			raw[(y * w + x) * 3 + 2] = int(p.b * 255) & 0xff;
		}
	return raw;
}
