#pragma once
#include "application/core/intern/image/image.h"
#include "glm/glm.hpp"
#include <vector>
#include <memory>

class renderer
{
public:
	renderer()=default;
	void OnResize(uint32_t width, uint32_t height);
	void render();
	std::shared_ptr<Image> GetFinalImage() const { return m_FinalImage; };
private:
	glm::vec3 PerPixel(glm::vec2 coord);
private:
	uint32_t m_renderwidth, m_renderheight=1 ;
	std::vector<glm::vec3>m_rawbuffer;
	std::shared_ptr<Image>m_FinalImage;
};
