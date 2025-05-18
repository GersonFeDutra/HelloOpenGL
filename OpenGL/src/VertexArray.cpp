#include "VertexArray.hpp"
#include "Util.hpp"


VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererId));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererId));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	bind();
	vb.bind();

	const auto& elements = layout.getElements();

	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		// Tells Open GL the layout of our attribute
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void *)offset));
		// sets the layout from the bounded buffer and link it to the vao by the index at 0

		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(m_RendererId));
}

void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}
