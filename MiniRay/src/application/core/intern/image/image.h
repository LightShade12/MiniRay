#pragma once
#include <string>
#include "glad/glad.h"
#include <vector>
#include "glm/glm.hpp"

class Image
{
public:

	Image(std::string_view path);
	Image(uint32_t width, uint32_t height, GLenum format, const void* data = nullptr);
	~Image();

	void initialiseGPUtexdata(const void* data);
	void updateGPUData(const std::vector<glm::vec3>& buffer, uint32_t width, uint32_t height);
	GLuint GetGLTexID() const { return m_textureID; };
	void Resize(uint32_t width, uint32_t height);;
	uint32_t GetWidth() const { return m_width; }
	uint32_t GetHeight() const { return m_height; }

private:
	uint32_t m_width = 0, m_height = 0;
	GLuint m_textureID = NULL;
	std::string m_filepath;
	size_t m_AlignedSize = 0;
	GLenum m_format = GL_RGB32F;
};