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

	friend std::ostream& operator<<(std::ostream& os, Vec4<T> v) {
		return os << v.r << ", " << v.g << ", " << v.b << ", " << v.a;
	}
};
