#pragma once

#include "types.hpp"

#include <iostream>

namespace rt {

	class Color {
	public:
		explicit Color();
		explicit Color(f32 v);
		explicit Color(f32 r, f32 g, f32 b);

		bool hasNaNs() const;

		friend std::ostream& operator<<(std::ostream& out, const Color& color);

		Color gammaCorrected(f32 gamma) const;
		Color& operator+=(const Color& other);
		Color& operator*=(const Color& other);
		Color& operator/=(f32 scalar);

		static Color lerp(const Color& a, const Color& b, f32 t);

	public:
		f32 r, g, b;
	};

	static const Color BLACK = Color(0.0f);
	static const Color WHITE = Color(1.0f);

} // namespace rt
