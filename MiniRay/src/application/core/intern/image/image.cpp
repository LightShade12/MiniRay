#include "image.h"
#include "stb/stb_image.h"
#include "opengl/glutils.h"
#include "stb/stb_image.h"
Image::Image(std::string_view path) :m_filepath(path)
{
	uint8_t* data = NULL;//temp use
	int width, height, channels;

	if (stbi_is_hdr(m_filepath.c_str()))
	{
		data = (uint8_t*)stbi_loadf(m_filepath.c_str(), &width, &height, &channels, 0);
		m_format = GL_RGBA32F;
	}
	else
	{
		data = stbi_load(m_filepath.c_str(), &width, &height, &channels, 0);
		m_format = GL_RGBA;
	}
	m_width = width;
	m_height = height;

	//AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format));
	uploadDatatoGPU(data);
	stbi_image_free(data);
}

Image::Image(uint32_t width, uint32_t height, GLenum format, const void* data)
	: m_width(width), m_height(height), m_format(format)
{
	if (data)
		uploadDatatoGPU(data);
}

Image::~Image()
{
	glDeleteTextures(1, &m_textureID);
}

void Image::uploadDatatoGPU(const void* data)
{
	GLbytestotexture(m_textureID, data, m_width, m_height, m_format);
}
