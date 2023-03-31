#pragma once

#include "../core/core.hpp"

#include <string>
#include <memory>

namespace rt {

	struct OptionsData {

	};

	class Options {
	public:
		Options() = delete;
		Options(const OptionsData& data);
		static std::unique_ptr<Options> FromFile(const std::string& filename);
	};

} // namespace rt
