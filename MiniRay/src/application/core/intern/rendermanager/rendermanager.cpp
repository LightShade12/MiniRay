#include "rendermanager.h"

rendermanager::rendermanager(GLuint width,GLuint height):m_width(width),m_height(height)
{
}

void rendermanager::Resize(uint32_t width, uint32_t height)
{
	if (buffersize / 3 != (m_height * m_width)) {

		buffersize = m_height * m_width* 3;
		rawframebuffer.resize(buffersize, 10);
		accumulationbuffer.resize(buffersize / 3, glm::vec3(1, 1, 1));

	}
}
