#include "options.hpp"

#include <fstream>
#include <iostream>

namespace rt {

	Options::Options(const OptionsData& data) {

	}

	std::unique_ptr<Options> Options::FromFile(const std::string& filename) {
		std::ifstream in(filename);
		if (!in.is_open()) {
			std::cerr << "Unable to open options file:" << filename << "\n";
			return std::unique_ptr<Options>(nullptr);
		}
		return std::make_unique<Options>(OptionsData{});
	}

} // namespace rt
