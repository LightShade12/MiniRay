#pragma once
#include <string>
#include "glad/glad.h"

class Image
{
public:

	Image(std::string_view path);
	Image(uint32_t width, uint32_t height, GLenum format, const void* data);
	~Image();

	void uploadDatatoGPU(const void* data);
	GLuint GetGLTexID() const { return m_textureID; };

	uint32_t GetWidth() const { return m_width; }
	uint32_t GetHeight() const { return m_height; }

private:
	void AllocateMemory(uint64_t size);
	void Release();
private:
	uint32_t m_width = 0, m_height = 0;
	GLuint m_textureID = NULL;
	std::string m_filepath;
	size_t m_AlignedSize = 0;
	GLenum m_format;
};