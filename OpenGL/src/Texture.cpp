#include <GL/glew.h>
#include <GLFW/glfw3.h>

// TODO -> add vendor to compiler include paths
#include "stb_image/stb_image.h"

#include "Texture.hpp"


Texture::Texture(const std::string& path) : m_RendererId(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	
	GLCall(glGenTextures(1, &m_RendererId));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}


Texture::~Texture()
{
	//stbi_image_free(m_LocalBuffer); // already deleted on constructor
	GLCall(glDeleteTextures(1, &m_RendererId));
}


void Texture::bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));
}


void Texture::unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
