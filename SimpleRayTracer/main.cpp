#include <sstream>
#include <fstream>
#include <iostream>

struct Options {
	std::string outFile{};
	int outw = 0, outh = 0;
};

struct Scene {
	std::string sceneFile{};
	Options options;
};

static std::ostream& operator <<(std::ostream& out, const Options& options) {
	out << "[Options]:\n"
		<< "Out file: " << options.outFile << "\n"
		<< "Size: " << options.outw << " x " << options.outh << "\n";
	return out;
}

static std::ostream& operator <<(std::ostream& out, const Scene& scene) {
	out << "[Scene]:\n"
		<< "Scene file: " << scene.sceneFile << "\n"
		<< scene.options;
	return out;
}

Scene parseArgs(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Input scene required as 1st parameter\n";
		exit(EXIT_FAILURE);
	}
	std::string sceneFile = argv[1];
	std::ifstream in(sceneFile);
	if (!in.is_open()) {
		std::cerr << "Unable to open file: " << sceneFile << "\n";
		exit(EXIT_FAILURE);
	}
	Scene scene;
	in >> scene.options.outFile;
	in >> scene.options.outw >> scene.options.outh;
	if (scene.options.outw <= 0 || scene.options.outh <= 0) {
		std::cerr << "Invalid (negative) size: " << scene.options.outw << " x " << scene.options.outh << "\n";
		exit(EXIT_FAILURE);
	}
	return scene;
}

struct Pixel {
	float r, g, b;
};

Pixel* process(const Scene& scene) {
	const int w = scene.options.outw, h = scene.options.outh;
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

void writeTGA(const std::string& filename, int w, int h, uint8_t* pixels) {
	// Write output as TGA image format
	// https://en.wikipedia.org/wiki/Truevision_TGA
	// https://stackoverflow.com/questions/16636311/what-is-the-simplest-rgb-image-format
	std::ofstream out(filename, std::ios::binary);

	/*
	Field no.   Length     Field name                Description
	1           1 byte     ID length                 Length of the image ID field
	2           1 byte     Color map type            Whether a color map is included
	3           1 byte     Image type                Compression and color types
	4           5 bytes    Color map specification   Describes the color map
	5           10 bytes   Image specification       Image dimensions and format
	*/
	uint8_t tgameta[18] = { {0} };
	tgameta[02] = 2; // uncompressed true-color image
	tgameta[12] = (w >> 0) & 0xff;
	tgameta[13] = (w >> 8) & 0xff;
	tgameta[14] = (h >> 0) & 0xff;
	tgameta[15] = (h >> 8) & 0xff;
	tgameta[16] = 24; // bits per pixel
	tgameta[17] = 0b00100000;

	out.write(reinterpret_cast<char*>(tgameta), sizeof(tgameta));
	out.write(reinterpret_cast<char*>(pixels), sizeof(uint8_t) * 3 * w * h);
}

void openFileInPaint(const std::string& filename) {
	std::ostringstream cmd;
	cmd << "start paintdotnet:\"" << filename << "\"";
	std::cout << "CMD: " << cmd.str() << "\n";
	system(cmd.str().c_str());
}

int main(int argc, char* argv[]) {
	///////////////////////////////
	Scene scene = parseArgs(argc, argv);
	std::cout << scene;
	///////////////////////////////
	std::cout << "\n[Processing]\n";
	Pixel* pixels = process(scene);
	///////////////////////////////
	std::cout << "\n[Writing]\n";
	uint8_t* rawPixels = convertToRawPixels(pixels, scene.options.outw, scene.options.outh);
	writeTGA(scene.options.outFile, scene.options.outw, scene.options.outh, rawPixels);
	///////////////////////////////
	std::cout << "\n[Executing]\n";
	openFileInPaint(scene.options.outFile);
	return EXIT_SUCCESS;
}
