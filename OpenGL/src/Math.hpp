#pragma once

#include "Util.hpp"
#include <cstdlib>

struct Vec2 {
	float x, y;

	float& operator[](std::size_t i) {
		my_assert(i >= 0 && i < 2);
		return reinterpret_cast<float*>(this)[i];
	}
};

struct Vec3 {
	float x, y, z;

	float& operator[](std::size_t i) {
		my_assert(i >= 0 && i < 3);
		return reinterpret_cast<float*>(this)[i];
	}
};

struct Vec4 {
	union {
		struct {
			float r, g, b, a;
		};
		struct {
			float v1, v2, v3, v4;
		};
	};

	float& operator[](std::size_t i) {
		my_assert(i >= 0 && i < 4);
		return reinterpret_cast<float*>(this)[i];
	}
};
