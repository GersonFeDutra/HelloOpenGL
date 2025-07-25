#pragma once

#include "glm/glm.hpp"

using Vector2 = glm::vec2;
using Vector2_double = glm::dvec2;
using Vector2_int = glm::ivec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using Matrix4 = glm::mat4;

typedef Vector2 TrianglePositions[3];
struct Quadrilateral {
	struct Vertex {
		Vector2 position;
		Vector2 textureCoords;
	};
	typedef Vertex Vertexes[4];

	Vertexes vertexes;
	unsigned int indices[6];
};

using Color = Vector4;

namespace colors {
	constexpr Color white = { 1.0f, 1.0f, 1.0f, 1.0f };
	constexpr Color black = { 0.0f, 0.0f, 0.0f, 1.0f };
	constexpr Color default_clear_color = { .2f, .3f, .8f, 1.0f };
};