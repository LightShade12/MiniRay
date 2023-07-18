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
			glm::vec3 color = RayGen(x, y);
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

//Executes Perpixel; currently also handles shading
glm::vec3 renderer::RayGen(uint32_t x, uint32_t y)
{
	//ray
	Ray ray;
	ray.orig = m_ActiveCamera->GetPosition();
	ray.dir = m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

	//shading variables
	glm::vec3 light(0);
	glm::vec3 contribution(1.0f);//models semi spectral absorption;should be 1

	//seed generation
	uint32_t seed = x + y * m_FinalImage->GetWidth();//local pixel seed
	seed *= m_FrameIndex;

	//recursive ray generation & shading
	for (int i = 0; i < m_Settings.Bounces; i++)
	{
		seed += i;

		HitPayload payload = TraceRay(ray);//1

		//Miss shader color------------------------------
		if (payload.HitDistance < 0)
		{
			glm::vec3 skycolor(0.6, 0.7, 0.9);
			light += skycolor * contribution;
			break;
		}

		//closestHit shader color------------------------------
		const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];
		const Material& material = m_ActiveScene->Materials[sphere.MaterialIndex];//material selection

		//light += material.GetEmmision();
		contribution *= material.Albedo;

		//new ray generation
		ray.orig = payload.WorldPosition + (payload.WorldNormal * rayEpsilon);

		if (m_Settings.mt1997_Random)
			ray.dir = glm::normalize(payload.WorldNormal + Random::InUnitSphere());
		else
			ray.dir = glm::normalize(payload.WorldNormal + RayTraceIntern::InUnitSphere(seed));
	}

	return light;
};

//shoots ray into the scene; engine responsible for traversing the ray throughout the scene
HitPayload renderer::TraceRay(const Ray& ray)
{
	//TODO:tranverse accel structures here
	//initialise working payload
	HitPayload WorkingPayload;
	WorkingPayload.ObjectIndex = -1;//object index of closest sphere
	WorkingPayload.HitDistance = std::numeric_limits<float>::max();

	//looping over scene objects
	//im future, with tlas and blas or simple bvh, this loop might iterate over triangles of the bottom most node in accel tree inside the intersection shader
	for (size_t i = 0; i < m_ActiveScene->Spheres.size(); i++)
	{
		WorkingPayload=Intersection(ray, (int)i, WorkingPayload);//in future it will take bottom most "node", payload and ray
	}
	
	//branched shaders; WHO INVOKES THESE SHADERS? INTERSECTION?
	if (WorkingPayload.ObjectIndex < 0) return Miss(ray);//MissShader

	return ClosestHit(ray, WorkingPayload.HitDistance, WorkingPayload.ObjectIndex);//ClosestHitShader
}

//closesthitshader; configures variables required for shading
//NOTE: its not generalized; integrated with sphere
HitPayload renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex)
{
	//setup
	HitPayload payload;
	payload.HitDistance = hitDistance;
	payload.ObjectIndex = objectIndex;

	const Sphere& closestsphere = m_ActiveScene->Spheres[objectIndex];
	
	//calculation
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

HitPayload renderer::Intersection(const Ray& ray, int objectindex, const HitPayload& incomingpayload)
{
	//setup
	HitPayload payload=incomingpayload;
	const Sphere& sphere = m_ActiveScene->Spheres[objectindex];
	glm::vec3 origin = ray.orig - sphere.Position;

	//calculation
	float a = glm::dot(ray.dir, ray.dir);
	float b = 2.0f * glm::dot(origin, ray.dir);
	float c = glm::dot(origin, origin) - sphere.Radius * sphere.Radius;

	float discriminant = b * b - 4.0f * a * c;

	//miss
	if (discriminant < 0) {
		return payload;
	}

	//not miss
	//float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
	float closest_t = (-b - glm::sqrt(discriminant)) / (2.0f * a);

	//if closest intersection
	if (closest_t > 0 && closest_t < incomingpayload.HitDistance) {
		payload.HitDistance = closest_t;
		payload.ObjectIndex = objectindex;
	}

	return payload;
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