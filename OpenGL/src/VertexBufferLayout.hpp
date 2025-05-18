#pragma once

#include "Util.hpp"
#include <vector>

// REFACTOR
#include <GL/glew.h> // Only if platforms supports OpenGL specification

// REFACTOR
// Open GL may have a different size for types
//typedef unsigned int GLuint;
typedef float GLfloat;

//#define GL_BOOL 0x8B56
//#define GL_BYTE 0x1400
//#define GL_UNSIGNED_BYTE 0x1401
//#define GL_SHORT 0x1402
//#define GL_UNSIGNED_SHORT 0x1403
//#define GL_INT 0x1404
//#define GL_UNSIGNED_INT 0x1405
//#define GL_FLOAT 0x1406
//#define GL_2_BYTES 0x1407
//#define GL_3_BYTES 0x1408
//#define GL_4_BYTES 0x1409
//#define GL_DOUBLE 0x140A
//#define GL_TRUE 1
//#define GL_FALSE 0


struct VertexBufferElement
{
	unsigned int type; // GLenum
	unsigned int count; // GLuint
	unsigned char normalized; // GLboolean

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type) {
			case GL_FLOAT:			return sizeof(GLfloat);
			case GL_UNSIGNED_INT:	return sizeof(GLuint);
			case GL_UNSIGNED_BYTE:	return sizeof(GLubyte);
			default: my_assert(false);
		}
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout() : m_Stride(0) {};


	template<typename T>
	void push(unsigned int count, unsigned char normalized = GL_FALSE)
	{
		static_assert(false); // will be handled by template speciallizations
	}

	template<>
	void push<float>(unsigned int count, unsigned char normalized)
	{
		m_Elements.push_back({GL_FLOAT, count, normalized});
		m_Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count, unsigned char normalized)
	{
		m_Elements.push_back({GL_UNSIGNED_INT, count, normalized});
		m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void push<unsigned char>(unsigned int count, unsigned char _normalized)
	{
		m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
		m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	//inline const& std::vector<VertexBufferElement> getElements() const { return m_Elements; }
	inline const std::vector<VertexBufferElement> getElements() const { return m_Elements; }

	inline unsigned int getStride() const { return m_Stride; }
};
