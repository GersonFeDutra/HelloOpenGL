#pragma once


class IndexBuffer
{
private:
	unsigned int m_RendererId;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const; // Select (bind) the buffer in the GPU.
	void unbind() const;

	inline unsigned int getCount() const { return m_Count; }
};
