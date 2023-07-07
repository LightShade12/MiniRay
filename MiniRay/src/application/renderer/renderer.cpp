#include "renderer.h"

void renderer::render()
{
	for (int i = 0; i < m_FinalImage->GetWidth() * m_FinalImage->GetHeight(); i++) {
		m_rawbuffer[i] = glm::vec3(1, 0, 1);//for some reason it works like its normalised; maybe tonemapper will fix?
	}
	m_FinalImage->updateGPUData(m_rawbuffer, m_FinalImage->GetWidth(), m_FinalImage->GetHeight());
}

glm::vec3 renderer::PerPixel(glm::vec2 coord)
{
}

void renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		// No resize necessary
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_renderwidth = width;
		m_renderheight = height;
		m_FinalImage->Resize(width, height);
		m_rawbuffer.resize(width * height, glm::vec3(0));
	}
	else
	{
		m_rawbuffer.resize(width * height, glm::vec3(0));//keep some data so gpu can be init
		m_FinalImage = std::make_shared<Image>(width, height, GL_RGB32F, m_rawbuffer.data());
	}
}