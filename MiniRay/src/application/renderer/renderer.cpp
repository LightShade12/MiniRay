#include "renderer.h"

void renderer::render()
{
	for (int y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (int x = 0; x < m_FinalImage->GetWidth(); x++) {
			glm::vec2 coord = { (float)x / m_FinalImage->GetWidth(),(float)y / m_FinalImage->GetHeight() };
			coord.x *= ((float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight());//aspect ratio
			coord.x = coord.x * 2.0f - ((float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight());//remap with aspect
			coord.y = coord.y * 2.0f - 1.0f;//remap
			m_rawbuffer[x + y * m_FinalImage->GetWidth()] = PerPixel(coord);//for some reason it works like its normalised; maybe tonemapper will fix?
		}
	}
	m_FinalImage->updateGPUData(m_rawbuffer, m_FinalImage->GetWidth(), m_FinalImage->GetHeight());
}

glm::vec3 renderer::PerPixel(glm::vec2 coord)
{
	glm::vec3 rayorig(0,0,1);
	glm::vec3 raydir(coord.x, coord.y, -1);
	float radius = 0.5f;

	float a = glm::dot(raydir, raydir);
	float b = 2.0f * glm::dot(rayorig, raydir);
	float c = glm::dot(rayorig, rayorig) - radius * radius;

	float discriminant = b * b - 4.0f * a * c;

	if (discriminant < 0) {
		return glm::vec3(0);
	}

	float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
	float closest_t = (-b - glm::sqrt(discriminant)) / (2.0f * a);

	glm::vec3 hitpoint = rayorig + raydir * closest_t;

	glm::vec3 normal = glm::normalize(hitpoint);

	glm::vec3 lightdir = glm::normalize(glm::vec3(-1));//why normalize?

	float d = glm::max(glm::dot(normal, -lightdir), 0.0f);

	glm::vec3 spherealbedo = { 1,0,1 };
	spherealbedo *= d;
	//return glm::vec3(coord.x, coord.y, 0);
	return glm::vec3(spherealbedo);
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