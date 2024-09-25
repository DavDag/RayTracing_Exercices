#pragma once

#include "../core/core.hpp"

namespace rt {

	class Image {
	public:
		Image(i32 w, i32 h);

		void set(i32 px, i32 py, Color pixel);
		Color get(i32 px, i32 py) const;
		const u8* rgb() const;

		Color add(i32 px, i32 py, Color pixel);
		Color getAcc(i32 px, i32 py) const;

		i32 width() const;
		i32 height() const;

		void save(const std::string& filename) const;
		void open(const std::string& filename) const;

		f32 percentage() const;
		void updatePercentage(f32 percentage);

	private:
		i32 _w, _h;
		u8* _data;
		Color* _acc;
		f32 _percentage;
	};

} // namespace rt
