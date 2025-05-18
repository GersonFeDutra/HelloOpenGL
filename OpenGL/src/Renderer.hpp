#pragma once

#include <GL/glew.h>

#include "VertexArray.hpp"
#include "Shader.hpp"
#include "IndexBuffer.hpp"


class Renderer
{
public:
	void clear() const;
	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
