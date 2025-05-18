#pragma once

#include <string>

#include "Util.hpp"

class Texture
{
private:
	unsigned int m_RendererId;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height;
	int m_BPP; // Bytes Per Pixel
public:
	Texture(const std::string& path);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Width; }
};
