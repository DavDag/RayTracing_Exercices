#pragma once

#include "../core/core.hpp"

namespace rt {

	class Image {
	public:
		Image(i32 w, i32 h);

		void set(i32 px, i32 py, Pixel pixel);
		Pixel get(i32 px, i32 py) const;
		const u8* rgb() const;

		i32 width() const;
		i32 height() const;

		void save(const std::string& filename) const;
		void open(const std::string& filename) const;

	private:
		i32 _w, _h;
		u8* _data;
	};

} // namespace rt
