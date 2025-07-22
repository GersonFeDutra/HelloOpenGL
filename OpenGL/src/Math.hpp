#pragma once

#include "Util.hpp"
#include <cstdlib>
#include <iostream>

template <typename T = float>
struct Vec2 {
	T x, y;

	T& operator[](std::size_t i) {
		my_assert(i >= 0 && i < 2);
		return reinterpret_cast<T*>(this)[i];
	}

	template <typename S>
	operator Vec2<S>() {
		return { static_cast<S>(x), static_cast<S>(y) };
	}

	Vec2 operator+() const { return *this; }
	Vec2 operator-() const { return { -x, -y }; }
	Vec2 operator-(Vec2 v) const { return { x - v.x, y - v.y }; }
	Vec2 operator+(Vec2 v) const { return { x + v.x, y + v.y }; }
	Vec2 operator*(float scalar) const { return { x * scalar, y * scalar }; }
	Vec2 operator*=(float scalar) { return *this = *this * scalar; }
	Vec2 operator+=(Vec2 v) { return *this = *this + v; }
	Vec2 operator-=(Vec2 v) { return *this = *this - v; }

	friend std::ostream& operator<<(std::ostream& os, Vec2<T> v) {
		return os << v.x << ", " << v.y;
	}
};

template <typename T = float>
struct Vec3 {
	union {
		struct {
			T x, y, z;
		};
		struct {
			T r, g, b;
		};
	};

	T& operator[](std::size_t i) {
		my_assert(i >= 0 && i < 3);
		return reinterpret_cast<T*>(this)[i];
	}

	template <typename S>
	operator Vec3<S>() {
		return { static_cast<S>(x), static_cast<S>(y), static_cast<S>(z) };
	}

	Vec3 operator+() const { return *this; }
	Vec3 operator-() const { return { -x, -y, -z }; }
	Vec3 operator-(Vec3 v) const { return { x - v.x, y - v.y, z - v.z }; }
	Vec3 operator+(Vec3 v) const { return { x + v.x, y + v.y, z + v.z }; }
	Vec3 operator*(float scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Vec3 operator*=(float scalar) { return *this = *this * scalar; }
	Vec3 operator+=(Vec3 v) { return *this = *this + v; }
	Vec3 operator-=(Vec3 v) { return *this = *this - v; }

	friend std::ostream& operator<<(std::ostream& os, Vec3<T> v) {
		return os << v.x << ", " << v.y << ", " << v.z;
	}
};

template <typename T = float>
struct Vec4 {
	union {
		struct {
			T r, g, b, a;
		};
		struct {
			T v1, v2, v3, v4;
		};
	};

	T& operator[](std::size_t i) {
		my_assert(i >= 0 && i < 4);
		return reinterpret_cast<T*>(this)[i];
	}

	template <typename S>
	operator Vec4<S>() {
		return { static_cast<S>(r), static_cast<S>(g), static_cast<S>(b), static_cast<S>(a) };
	}

	Vec4 operator+() const { return *this; }
	Vec4 operator-() const { return { -r, -g, -b, -a }; }
	Vec4 operator-(Vec4 v) const { return { r - v.r, g - v.g, b - v.b, a - v.a }; }
	Vec4 operator+(Vec4 v) const { return { r + v.r, g + v.g, b + v.b, a + v.a }; }
	Vec4 operator*(float scalar) const { return { r * scalar, g * scalar, b * scalar, a * scalar}; }
	Vec4 operator*=(float scalar) { return *this = *this * scalar; }
	Vec4 operator+=(Vec4 v) { return *this = *this + v; }
	Vec4 operator-=(Vec4 v) { return *this = *this - v; }

	friend std::ostream& operator<<(std::ostream& os, Vec4<T> v) {
		return os << v.r << ", " << v.g << ", " << v.b << ", " << v.a;
	}
};
