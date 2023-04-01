#include "options.hpp"

namespace rt {

	Options::Options(const OptionsData& data):
		w(data.outw), h(data.outh),
		samples(data.samples), maxdepth(data.maxdepth)
	{

	}

	std::shared_ptr<Options> Options::FromFile(const std::string& filename) {
		std::ifstream in(filename);
		if (!in.is_open()) {
			std::cerr << "Unable to open options file:" << filename << "\n";
			return std::shared_ptr<Options>(nullptr);
		}
		//
		OptionsData data {
			.outw = 0,
			.outh = 0,
			.samples = 0,
			.maxdepth = 0,
		};
		//
		std::string line{}, tag{}, equal{};
		while (std::getline(in, line)) {
			if (line.size() == 0) continue;
			if (line[0] == '#') continue;
			std::istringstream ss(line);
			ss >> tag >> equal;
			switch (crc32(tag)) {
			case crc32("output"):
				ss >> data.outw >> data.outh;
				break;
			case crc32("samples"):
				ss >> data.samples;
				break;
			case crc32("maxdepth"):
				ss >> data.maxdepth;
				break;
			default:
				std::cerr << "Unrecognized tag: " << tag << "\n";
				break;
			}
		}
		//
		return std::make_unique<Options>(data);
	}

} // namespace rt
