#pragma once
#include "application/core/intern/image/image.h"

#include <vector>
#include <memory>

class renderer
{
public:
	renderer();
	void OnResize(uint32_t width, uint32_t height);
	void render();
	std::shared_ptr<Image> GetFinalImage() const { return m_FinalImage; };
	uint32_t m_renderwidth, m_renderheight=1 ;
private:
	std::vector<GLubyte>m_rawbuffer;
	std::shared_ptr<Image>m_FinalImage;
};
