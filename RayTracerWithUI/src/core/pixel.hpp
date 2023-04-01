#pragma once

#include "types.hpp"

#include <iostream>

namespace rt {

	class Pixel {
	public:
		explicit Pixel();
		explicit Pixel(f32 v);
		explicit Pixel(f32 r, f32 g, f32 b);

		Pixel gammaCorrected(f32 gamma) const;
		Pixel& operator+=(const Pixel& other);
		Pixel& operator/=(f32 scalar);

		std::ostream& operator<<(std::ostream& out) const;

		static Pixel lerp(const Pixel& a, const Pixel& b, f32 t);

	public:
		f32 r, g, b;
	};

} // namespace rt
