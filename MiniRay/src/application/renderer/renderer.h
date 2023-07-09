#pragma once
#include "application/core/intern/image/image.h"
#include "application/camera/camera.h"
#include "glm/glm.hpp"
#include <vector>
#include <memory>

struct Ray {
	glm::vec3 orig = glm::vec3(0);
	glm::vec3 dir = glm::vec3(0);
};

class renderer
{
public:
	renderer() = default;
	void OnResize(uint32_t width, uint32_t height);
	void render(const Camera& scenecam);
	std::shared_ptr<Image> GetFinalImage() const { return m_FinalImage; };
private:
	glm::vec3 TraceRay(const Ray& ray);
private:
	uint32_t m_renderwidth, m_renderheight = 1;
	std::vector<glm::vec3>m_rawbuffer;
	std::shared_ptr<Image>m_FinalImage;
};
