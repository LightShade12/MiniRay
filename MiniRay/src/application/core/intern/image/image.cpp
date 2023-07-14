#include "image.h"
#include "stb/stb_image.h"
#include "opengl/glutils.h"
#include "stb/stb_image.h"

Image::Image(std::string_view path) :m_filepath(path)
{
	int width, height, channels;
	GLubyte* data = nullptr;

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
	initialiseGPUtexdata(data);
}

Image::Image(uint32_t width, uint32_t height, GLenum format, const void* data)
	: m_width(width), m_height(height), m_format(format)
{
	if (data)
		initialiseGPUtexdata(data);
}

Image::~Image()
{
	glDeleteTextures(1, &m_textureID);
}

void Image::initialiseGPUtexdata(const void* data)
{
	//init gpu texbuffer and init the handle i.e texID
	GLbytestotexture(m_textureID, data, m_width, m_height, m_format);
}

void Image::updateGPUData(const std::vector<glm::vec3>& buffer, uint32_t width, uint32_t height)
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height, 0, GL_RGB, bytesPerChannel(m_format), buffer.data());
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Image::Resize(uint32_t width, uint32_t height)
{
	//TODO: check if alt ver is required if bug
	//if  (m_textureID && m_width == width && m_height == height)
	if (m_width == width && m_height == height)
		return;

	// TODO: max size?

	m_width = width;
	m_height = height;

	/*glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, GL_RGB, bytesPerChannel(m_format), m_data);
	glBindTexture(GL_TEXTURE_2D, 0);*/
}