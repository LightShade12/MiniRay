#include "renderer.h"
#include <iostream>
#include "UtilsCommon/random.h"
#include "UtilsCommon/RayTraceUtils.h"

void renderer::render(const Scene& scene, const Camera& camera)
{
	m_ActiveScene = &scene;
	m_ActiveCamera = &camera;

	if (m_FrameIndex == 1)
		memset(m_accumulationbuffer.data(), 0, m_FinalImage->GetHeight() * m_FinalImage->GetWidth() * sizeof(glm::vec3));

	if (m_FrameIndex == m_Settings.MaxSamplesLimit)
		return;

	for (int y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (int x = 0; x < m_FinalImage->GetWidth(); x++) {
			glm::vec3 color = PerPixel(x, y);
			//glm::vec2 coord = { (float)x / m_FinalImage->GetWidth(),(float)y / m_FinalImage->GetHeight() };
			//coord.x *= ((float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight());//aspect ratio
			//coord.x = coord.x * 2.0f - ((float)m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight());//remap with aspect
			//coord.y = coord.y * 2.0f - 1.0f;//remap
			m_accumulationbuffer[x + y * m_FinalImage->GetWidth()] += color;
			glm::vec3 AccumulatedColor = m_accumulationbuffer[x + y * m_FinalImage->GetWidth()];
			AccumulatedColor /= (float)m_FrameIndex;
			m_rawbuffer[x + y * m_FinalImage->GetWidth()] = AccumulatedColor;//for some reason it works like its normalised; maybe tonemapper will fix?
		}
	}
	m_FinalImage->updateGPUData(m_rawbuffer, m_FinalImage->GetWidth(), m_FinalImage->GetHeight());

	if (m_Settings.Accumulate)
		m_FrameIndex++;
	else
		m_FrameIndex = 1;//if at fID 10 then go back to 1 edit:wtf does this mean
}

float rayEpsilon = 0.001f;

//Raygen shader
glm::vec3 renderer::PerPixel(uint32_t x, uint32_t y)
{
	//TODO:tranverse accel structures here
	Ray ray;
	ray.orig = m_ActiveCamera->GetPosition();
	ray.dir = m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

	glm::vec3 light(0);

	glm::vec3 contribution(1.0f);//models semi spectral absorption;should be 1

	uint32_t seed = x + y * m_FinalImage->GetWidth();
	seed *= m_FrameIndex;

	for (int i = 0; i < m_Settings.Bounces; i++)
	{
		seed += i;

		HitPayload payload = TraceRay(ray);//1

		if (payload.HitDistance < 0)
		{
			glm::vec3 skycolor(0.6, 0.7, 0.9);
			//glm::vec3 skycolor(0);
			light += skycolor * contribution;
			break;
		}

		const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];
		const Material& material = m_ActiveScene->Materials[sphere.MaterialIndex];

		light += material.GetEmmision();
		contribution *= material.Albedo;

		ray.orig = payload.WorldPosition + (payload.WorldNormal * rayEpsilon);
		if (m_Settings.mt1997_Random)
			ray.dir = glm::normalize(payload.WorldNormal + Random::InUnitSphere());
		else
			ray.dir = glm::normalize(payload.WorldNormal + RayTraceIntern::InUnitSphere(seed));
	}

	return light;
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
		if (closest_t > 0 && closest_t < hitdist) {
			hitdist = closest_t;
			closestsphere = (int)i;
		}
	}

	//branched shaders
	if (closestsphere < 0) return Miss(ray);//MissShader

	return ClosestHit(ray, hitdist, closestsphere);//ClosestHitShader
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
		m_accumulationbuffer.resize(width * height, glm::vec3(0));
		ResetFrameIndex();
	}
	else
	{
		m_rawbuffer.resize(width * height, glm::vec3(0));//keep some data so gpu can be init
		m_accumulationbuffer.resize(width * height, glm::vec3(0));
		m_FinalImage = std::make_shared<Image>(width, height, GL_RGB32F, m_rawbuffer.data());
	}
}