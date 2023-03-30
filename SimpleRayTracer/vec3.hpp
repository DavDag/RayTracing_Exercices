#pragma once

#include "rnd.hpp"

#include <cmath>
#include <iostream>

using Scalar = double;

template<typename T>
class Vec3 {
public:
	explicit Vec3(): Vec3((T)0) {}
	explicit Vec3(T v): Vec3(v, v, v) {}
	Vec3(T x, T y, T z): x(x), y(y), z(z) { }

	Vec3<T> operator-() const {
		return Vec3<T>(-x, -y, -z);
	}

	Vec3<T>& operator+=(const Vec3<T>& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return (*this);
	}

	Vec3<T>& operator-=(const Vec3<T>& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return (*this);
	}

	Vec3<T>& operator*=(Scalar scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return (*this);
	}

	Vec3<T>& operator/=(Scalar scalar) {
		(*this) *= (1 / scalar);
		return (*this);
	}

	Scalar length() const {
		return std::sqrt(this->lengthSquared());
	}

	Scalar lengthSquared() const {
		return (Scalar)x * x + y * y + z * z;
	}

	Vec3<T> normalized() const {
		return Vec3<T>(*this) / this->length();
	}

	void normalize() {
		(*this) /= this->length();
	}

	Vec3<T> reflect(const Vec3<T>& surfNorm) const {
		return (*this) - Scalar(2.0) * dot(*this, surfNorm) * surfNorm;
	}

public:
	T x, y, z;
};

template<typename T>
static std::ostream& operator <<(std::ostream& out, const Vec3<T>& vec) {
	out << "[" << vec.x << "," << vec.y << "," << vec.z << "]";
	return out;
}

template<typename T>
static Vec3<T> operator+(const Vec3<T>& veca, const Vec3<T> vecb) {
	return Vec3<T>(veca.x + vecb.x, veca.y + vecb.y, veca.z + vecb.z);
}

template<typename T>
static Vec3<T> operator-(const Vec3<T>& veca, const Vec3<T> vecb) {
	return Vec3<T>(veca.x - vecb.x, veca.y - vecb.y, veca.z - vecb.z);
}

template<typename T>
static Vec3<T> operator*(const Vec3<T>& vec, Scalar scalar) {
	return Vec3<T>(T(vec.x * scalar), T(vec.y * scalar), T(vec.z * scalar));
}

template<typename T>
static Vec3<T> operator*(Scalar scalar, const Vec3<T>& vec) {
	return vec * scalar;
}

template<typename T>
static Vec3<T> operator/(const Vec3<T>& vec, Scalar scalar) {
	Scalar tmp = 1 / scalar;
	return vec * tmp;
}

template<typename T>
static Scalar dot(const Vec3<T>& veca, const Vec3<T>& vecb) {
	return (Scalar)veca.x * vecb.x + veca.y * vecb.y + veca.z * vecb.z;
}

template<typename T>
static Vec3<T> cross(const Vec3<T>& veca, const Vec3<T>& vecb) {
	return Vec3<T>(
		veca.y * vecb.z - veca.z * vecb.y,
		veca.z * vecb.x - veca.x * vecb.z,
		veca.x * vecb.y - veca.y * vecb.x
	);
}

template<typename T>
static Vec3<T> lerp(const Vec3<T>& veca, const Vec3<T>& vecb, Scalar t) {
	return veca * (Scalar(1.0) - t) + vecb * (t);
}

using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;
using Vec3i = Vec3<int>;
using Vec3u = Vec3<unsigned int>;

static Vec3f RandomVec3f(float min = 0.0f, float max = 1.0f) {
	return Vec3f(rndFloat(min, max), rndFloat(min, max), rndFloat(min, max));
}

static Vec3d RandomVec3d(double min = 0.0, double max = 1.0) {
	return Vec3d(rndDouble(min, max), rndDouble(min, max), rndDouble(min, max));
}

static Vec3f gamma(const Vec3f& vec, float gamma) {
	float exp = 1.0f / gamma;
	return Vec3f(std::powf(vec.x, exp), std::powf(vec.y, exp), std::powf(vec.z, exp));
}

static Vec3d gamma(const Vec3d& vec, double gamma) {
	double exp = 1.0 / gamma;
	return Vec3d(std::pow(vec.x, exp), std::pow(vec.y, exp), std::pow(vec.z, exp));
}

static bool IsZero(const Vec3f& vec) {
	constexpr float pos = +0.0001f;
	constexpr float neg = -0.0001f;
	return (vec.x < pos && vec.x > neg)
		&& (vec.y < pos && vec.y > neg)
		&& (vec.z < pos && vec.z > neg);
}

static bool IsZero(const Vec3d& vec) {
	constexpr double pos = +0.0000000001f;
	constexpr double neg = -0.0000000001f;
	return (vec.x < pos && vec.x > neg)
		&& (vec.y < pos && vec.y > neg)
		&& (vec.z < pos && vec.z > neg);
}
