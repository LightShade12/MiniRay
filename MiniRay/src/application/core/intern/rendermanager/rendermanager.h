#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glad/glad.h"
#include "opengl/glutils.h"


class rendermanager
{
public:
	rendermanager(GLuint width, GLuint height);
	~rendermanager();
	void Resize(uint32_t width, uint32_t height);
private:
	uint32_t m_width = 0, m_height = 0;
	size_t buffersize = 0;

	std::vector<framebuffer>m_framebuffers;
	std::vector<screenVAOVBO>m_screenVAOVBO;
	std::vector<GLfloat>rawframebuffer;
	std::vector<glm::vec3>accumulationbuffer;
};