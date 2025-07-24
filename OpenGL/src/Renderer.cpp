#include "Renderer.hpp"


void Renderer::clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}


void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	/* Bind all data to be used */
	// TODO -> Make re-binding only in debug mode

	shader.bind(); // binds a program so the GPU can render our data
	
	// the actual data that will be rendered
	va.bind();
	ib.bind(); // what vertices to render

	// Open GL access the vertexes with the Index Buffer to render the geometry properly using the shader program
	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}
