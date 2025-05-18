#include "Renderer.hpp"


void Renderer::clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}


void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	/* Bind all data to be used */
	shader.bind();
	va.bind();
	ib.bind();
	// TODO -> Make re-binding only in debug mode

	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}
