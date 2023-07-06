#include "renderer.h"

renderer::renderer()
{
	//arbitrary buffer
	m_rawbuffer.resize(120 * 120 * 3, 0);//init buffer for upload gpu init
}

void renderer::render()
{
	for (int i = 0; i < m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * 3; i = i + 3) {
		m_rawbuffer[i] = 255;//r
		m_rawbuffer[i + 1] = 0;//g
		m_rawbuffer[i + 2] = 255;//b
	}
	m_FinalImage->updateGPUData(m_rawbuffer, m_FinalImage->GetWidth(), m_FinalImage->GetHeight());
	//m_FinalImage->initialiseGPUtexdata(m_data);
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
		m_rawbuffer.resize(width * height * 3, 0);
	}
	else
	{
		m_rawbuffer.resize(width * height * 3, 0);
		m_FinalImage = std::make_shared<Image>(width, height, GL_RGB, m_rawbuffer.data());
	}
}