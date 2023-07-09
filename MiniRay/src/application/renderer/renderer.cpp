#include "renderer.h"
#include <iostream>

void renderer::render(const Scene& scene, const Camera& camera)
{
	m_ActiveScene = &scene;
	m_ActiveCamera = &camera;

	for (int y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (int x = 0; x < m_FinalImage->GetWidth(); x++) {
			//glm::vec2 coord = { (float)x / m_FinalImage->GetWidth(),(float)y / m_FinalImage->GetHeight() };
			//coord.x *= ((float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight());//aspect ratio
			//coord.x = coord.x * 2.0f - ((float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight());//remap with aspect
			//coord.y = coord.y * 2.0f - 1.0f;//remap
			m_rawbuffer[x + y * m_FinalImage->GetWidth()] = PerPixel(x, y);//for some reason it works like its normalised; maybe tonemapper will fix?
		}
	}
	m_FinalImage->updateGPUData(m_rawbuffer, m_FinalImage->GetWidth(), m_FinalImage->GetHeight());
}


//Raygen shader
glm::vec3 renderer::PerPixel(uint32_t x, uint32_t y)
{
	Ray ray;
	ray.orig = m_ActiveCamera->GetPosition();
	ray.dir = m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

	HitPayload payload = TraceRay(ray);

	if (payload.HitDistance < 0)
		return glm::vec3(0);

	glm::vec3 lightdir = glm::normalize(glm::vec3(-1));//why normalize?

	float lightIntensity = glm::max(glm::dot(payload.WorldNormal, -lightdir), 0.0f);

	const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];

	glm::vec3 spherealbedo = sphere.albedo;
	spherealbedo *= lightIntensity;

	return spherealbedo;
};

HitPayload renderer::TraceRay(const Ray& ray)
{
	int closestsphere = -1;
	float hitdist = std::numeric_limits<float>::max();

	for (size_t i = 0; i < m_ActiveScene->Spheres.size(); i++)
	{
		const Sphere& sphere = m_ActiveScene->Spheres[i];
		glm::vec3 origin = ray.orig - sphere.Position;

		float a = glm::dot(ray.dir, ray.dir);
		float b = 2.0f * glm::dot(origin, ray.dir);
		float c = glm::dot(origin, origin) - sphere.Radius * sphere.Radius;

		float discriminant = b * b - 4.0f * a * c;

		if (discriminant < 0) {
			//return glm::vec3(0);
			continue;
		}

		//float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
		float closest_t = (-b - glm::sqrt(discriminant)) / (2.0f * a);
		if (closest_t < hitdist) {
			hitdist = closest_t;
			closestsphere = (int)i;
		}
	}

	if (closestsphere < 0) return Miss(ray);

	return ClosestHit(ray, hitdist, closestsphere);
}

//closesthitshader
HitPayload renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex)
{
	HitPayload payload;
	payload.HitDistance = hitDistance;
	payload.ObjectIndex = objectIndex;

	const Sphere& closestsphere = m_ActiveScene->Spheres[objectIndex];

	glm::vec3 origin = ray.orig - closestsphere.Position;

	payload.WorldPosition = origin + ray.dir * hitDistance;

	payload.WorldNormal = glm::normalize(payload.WorldPosition);

	payload.WorldPosition += closestsphere.Position;//reset positiomn to origin

	return payload;
}
HitPayload renderer::Miss(const Ray& ray)
{
	HitPayload payload;
	payload.HitDistance = -1;
	return payload;
};


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