#pragma once


class VertexBuffer
{
private:
	unsigned int m_RendererId;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const; // Select (bind) the buffer in the GPU.
	void Unbind() const; // Select no buffer.
};
