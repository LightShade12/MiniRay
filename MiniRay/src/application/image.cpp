#include "image.h"
#include "creator/stb/stb_image.h"
#include "glutils.h"

Image::Image(std::string_view path) :m_filepath(path)
{
	uint8_t* stack_data = NULL;
	int width, height, channels;

	if (stbi_is_hdr(m_filepath.c_str()))
	{
		stack_data = (uint8_t*)stbi_loadf(m_filepath.c_str(), &width, &height, &channels, 0);
		m_format = GL_RGBA32F;
	}
	else
	{
		stack_data = stbi_load(m_filepath.c_str(), &width, &height, &channels, 0);
		m_format = GL_RGBA;
	}
	m_data = new uint8_t[m_width * m_height * sizeof(uint8_t) * channels]; m_data = stack_data;
	m_width = width;
	m_height = height;
	uploadDatatoGPU(m_data);
}

Image::Image(uint32_t width, uint32_t height, GLenum format, const void* data)
	: m_width(width), m_height(height), m_format(format)
{
	if (data)
		uploadDatatoGPU(data);
}

Image::~Image()
{
	delete[] m_data;
	glDeleteTextures(1, &m_textureID);
}

void Image::uploadDatatoGPU(const void* data)
{
	GLbytestotexture(m_textureID, data, m_width, m_height, m_format);
}