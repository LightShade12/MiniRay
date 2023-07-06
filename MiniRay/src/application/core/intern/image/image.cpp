#include "image.h"
#include "stb/stb_image.h"
#include "opengl/glutils.h"
#include "stb/stb_image.h"
Image::Image(std::string_view path) :m_filepath(path)
{
	int width, height, channels;

	if (stbi_is_hdr(m_filepath.c_str()))
	{
		m_data = (uint8_t*)stbi_loadf(m_filepath.c_str(), &width, &height, &channels, 0);
		m_format = GL_RGBA32F;
	}
	else
	{
		m_data = stbi_load(m_filepath.c_str(), &width, &height, &channels, 0);
		m_format = GL_RGBA;
	}
	m_width = width;
	m_height = height;

	//AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format));
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
	stbi_image_free(m_data);
	glDeleteTextures(1, &m_textureID);
}

void Image::uploadDatatoGPU(const void* data)
{
	GLbytestotexture(m_textureID, data, m_width, m_height, m_format);
}

void Image::Resize(uint32_t width, uint32_t height)
{
	if  (m_data && m_width == width && m_height == height)
		return;

	// TODO: max size?

	m_width = width;
	m_height = height;

	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, GL_RGB, bytesPerPixel(m_format), m_data);
	glBindTexture(GL_TEXTURE_2D, 0);
}
