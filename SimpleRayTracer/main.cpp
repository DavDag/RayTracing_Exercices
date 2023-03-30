#include <chrono>
#include <iostream>

#include "tga.hpp"
#include "raytracer.hpp"

struct Options {
	std::string sceneFile{};
	std::string configFile{};
	std::string outFile{};
	int outw = 0, outh = 0;
	int samples = 16, depthMax = 4;
};

static std::ostream& operator <<(std::ostream& out, const Options& options) {
	out << "Scene file: " << options.sceneFile << "\n"
		<< "Out file: " << options.outFile << "\n"
		<< "Size: " << options.outw << " x " << options.outh << "\n"
		<< "Samples: " << options.samples << "\n"
		<< "DepthMax: " << options.depthMax << "\n";
	return out;
}

Options parseArgs(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Input scene required as 1st parameter\n";
		exit(EXIT_FAILURE);
	}
	if (argc < 3) {
		std::cerr << "Config file required as 2nd parameter\n";
		exit(EXIT_FAILURE);
	}
	Options options;
	options.sceneFile = argv[1];
	options.configFile = argv[2];

	std::ifstream in(options.configFile);
	in >> options.outFile;
	in >> options.outw >> options.outh;
	in >> options.samples >> options.depthMax;

	return options;
}

int main(int argc, char* argv[]) {
	///////////////////////////////
	Options options = parseArgs(argc, argv);
	std::cout << options;
	///////////////////////////////
	auto tstart = std::chrono::high_resolution_clock::now();
	Scene scene = parseScene(options.sceneFile, options.outw, options.outh);
	Vec3f* pixels = process(scene, options.outw, options.outh, options.samples, options.depthMax);
	uint8_t* rawPixels = convertToRawPixels(pixels, options.outw, options.outh);
	writeTGA(options.outFile, options.outw, options.outh, rawPixels);
	auto tend = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(tend - tstart).count() << "ms";
	///////////////////////////////
	openFileInPaint(options.outFile, false);
	return EXIT_SUCCESS;
}
