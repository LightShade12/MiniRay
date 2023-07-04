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
	uint8_t* GetData() const { return m_data; };
	//void Resize(uint32_t width, uint32_t height);

	uint32_t GetWidth() const { return m_width; }
	uint32_t GetHeight() const { return m_height; }

private:
	uint8_t* m_data = nullptr;
	GLuint m_textureID = NULL;
	uint32_t m_width = 0, m_height = 0;
	std::string m_filepath;
	GLenum m_format;
};