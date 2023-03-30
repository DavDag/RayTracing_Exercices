#include <iostream>

#include "tga.hpp"
#include "raytracer.hpp"

struct Options {
	std::string sceneFile{};
	std::string outFile{};
	int outw = 0, outh = 0;
};

static std::ostream& operator <<(std::ostream& out, const Options& options) {
	out << "[Options]:\n"
		<< "Scene file: " << options.sceneFile << "\n"
		<< "Out file: " << options.outFile << "\n"
		<< "Size: " << options.outw << " x " << options.outh << "\n";
	return out;
}

Options parseArgs(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Input scene required as 1st parameter\n";
		exit(EXIT_FAILURE);
	}
	if (argc < 3) {
		std::cerr << "Output file required as 2nd parameter\n";
		exit(EXIT_FAILURE);
	}
	Options options;
	options.sceneFile = argv[1];
	options.outFile = argv[2];
	options.outw = (argc > 3) ? (strtol(argv[3], nullptr, 10)) : 300;
	options.outh = (argc > 4) ? (strtol(argv[4], nullptr, 10)) : 200;
	return options;
}

int main(int argc, char* argv[]) {
	///////////////////////////////
	Options options = parseArgs(argc, argv);
	std::cout << options;
	///////////////////////////////
	Pixel* pixels = process(options.sceneFile, options.outw, options.outh);
	uint8_t* rawPixels = convertToRawPixels(pixels, options.outw, options.outh);
	writeTGA(options.outFile, options.outw, options.outh, rawPixels);
	///////////////////////////////
	openFileInPaint(options.outFile, false);
	return EXIT_SUCCESS;
}
