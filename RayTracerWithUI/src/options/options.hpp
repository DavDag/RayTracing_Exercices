#pragma once

#include "../core/core.hpp"

namespace rt {

	struct OptionsData {
		i32 samples;
		i32 maxdepth;
	};

	class Options {
	public:
		Options() = delete;
		Options(const OptionsData& data);

		friend std::ostream& operator<<(std::ostream& out, const Options& options);

		static std::shared_ptr<Options> FromFile(const std::string& filename);

	public:
		const i32 samples;
		const i32 maxdepth;
	};

} // namespace rt
