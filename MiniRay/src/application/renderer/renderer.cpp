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
	//for (int i = 0; i < 1; i++)
	for (int i = 0; i < m_Settings.Bounces; i++)
	{
		seed += i;

		HitPayload payload = TraceRay(ray);//1
		//std::cerr << payload.WorldNormal.x << "," << payload.WorldNormal.y << "," << payload.WorldNormal.z << "\n";

		//Miss shader color------------------------------
		if (payload.HitDistance < 0)
		{
			glm::vec3 skycolor(0.6, 0.7, 0.9);
			light += skycolor * contribution;
			//light = skycolor;
			break;
		}
		//closestHit shader color------------------------------
		const auto& mesh = m_ActiveScene->Meshes[payload.ObjectIndex];
		const Material& material = m_ActiveScene->Materials[mesh.MaterialIndex];//material selection
		//light += material.GetEmmision();
		contribution *= material.Albedo;
		//light = (payload.WorldNormal);
		//std::cerr <<"Color:" << light.x << " , " << light.y << " , " << light.z << "\n";
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
	for (int i = 0; i < m_ActiveScene->Meshes.size(); i++)
	{
		for (int j = 0; j < m_ActiveScene->Meshes[i].m_triangles.size(); j++)
		{
			WorkingPayload = Intersection(ray, (int)i, WorkingPayload, j);//in future it will take bottom most "node", payload and ray
		}
	}

	//branched shaders; WHO INVOKES THESE SHADERS? INTERSECTION?
	if (WorkingPayload.ObjectIndex < 0) return Miss(ray);//MissShader

	return ClosestHit(ray, WorkingPayload.HitDistance, WorkingPayload.ObjectIndex, WorkingPayload.Polygonindex);//ClosestHitShader
}

//closesthitshader; configures variables required for shading
//NOTE: its not generalized; integrated with sphere
HitPayload renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex, int triangleindex)
{
	//setup
	HitPayload payload;
	payload.WorldNormal = { 1,1,1 };//not necessary
	payload.HitDistance = hitDistance;
	payload.ObjectIndex = objectIndex;
	//std::cerr << "\n";
	//std::cerr <<"obj index "<< objectIndex << "\n";
	//std::cerr <<"tri index "<< triangleindex << "\n";
	const Triangle& closestTriangle = m_ActiveScene->Meshes[objectIndex].m_triangles[triangleindex];

	//calculation
	//glm::vec3 origin = ray.orig - closestTriangle.Position;//add matrix translation code here

	//payload.WorldPosition = ray.orig + ray.dir * hitDistance;//originally used origin for translation
	payload.WorldPosition = ray.orig + ray.dir * payload.HitDistance;

	//if (closestTriangle.Normal == glm::vec3(0))
	if (true)
	{
		//std::cerr << "vert1: " << closestTriangle.vert1.x << "," << closestTriangle.vert1.y << "," << closestTriangle.vert1.z << "\n";
		//std::cerr << "vert2: " << closestTriangle.vert2.x << "," << closestTriangle.vert2.y << "," << closestTriangle.vert2.z << "\n";

		glm::vec3 edge1 = closestTriangle.vert1 - closestTriangle.vert0;
		//std::cerr << "edge1: " << edge1.x << "," << edge1.y << "," << edge1.z << "\n";

		glm::vec3 edge2 = closestTriangle.vert2 - closestTriangle.vert0;
		//std::cerr << "edge2: " << edge2.x << "," << edge2.y << "," << edge2.z << "\n";

		payload.WorldNormal = (glm::cross(edge2, edge1));//swap edges for ccw or cw

		//std::cerr << "rawnormal: " << payload.WorldNormal.x << "," << payload.WorldNormal.y << "," << payload.WorldNormal.z << "\n";
		payload.WorldNormal = glm::normalize(payload.WorldNormal);
	}
	//if (payload.WorldNormal.x == 0 && payload.WorldNormal.z == 0)
	//if(triangleindex==9&&objectIndex==0)
		//std::cerr << "finalnormal: " << payload.WorldNormal.x << "," << payload.WorldNormal.y << "," << payload.WorldNormal.z << "\n";
	/*else
		payload.WorldNormal = glm::normalize(closestTriangle.Normal);*/

		//payload.WorldNormal = glm::normalize(payload.WorldPosition);

		//payload.WorldPosition += closestTriangle.Position;//reset positiomn to origin

	return payload;
}

HitPayload renderer::Miss(const Ray& ray)
{
	HitPayload payload;
	payload.HitDistance = -1;
	return payload;
};

// Function to check if two floating-point numbers are almost equal within a small epsilon
bool nearlyEqual(float a, float b, float epsilon = 1e-8) {
	return fabs(a - b) < epsilon;
}

HitPayload renderer::Intersection(const Ray& ray, int objectindex, const HitPayload& incomingpayload, int polyonindex)
{
	//setup
	HitPayload payload = incomingpayload;
	const Triangle& triangle = m_ActiveScene->Meshes[objectindex].m_triangles[polyonindex];

	//calculation
	const float EPSILON = 1e-8;

	glm::vec3 edge1, edge2, h, s, q;
	float a, f, u, v;

	edge1 = triangle.vert1 - triangle.vert0;
	edge2 = triangle.vert2 - triangle.vert0;

	h = glm::cross(ray.dir, edge2);
	a = glm::dot(edge1, h);

	// Check if the ray is parallel to the triangle plane
	if (nearlyEqual(a, 0.0f, EPSILON))
		return payload;

	f = 1.0f / a;
	s = ray.orig - triangle.vert0;
	u = f * glm::dot(s, h);

	// Check if the intersection point is outside the triangle
	if (u < 0.0f || u > 1.0f)
		return payload;

	q = glm::cross(s, edge1);
	v = f * glm::dot(ray.dir, q);

	// Check if the intersection point is outside the triangle
	if (v < 0.0f || u + v > 1.0f)
		return payload;

	// Compute the t value, which represents the distance from the ray origin to the intersection point
	float closest_t = f * glm::dot(edge2, q);

	// Check if the intersection point is behind the ray origin or too far away
	if (closest_t < EPSILON)
		return payload;

	//not miss
	// Compute the intersection point
	//glm::vec3  intersectionPoint = ray.orig + closest_t * ray.dir;

	//if closest intersection
	if (closest_t > 0 && closest_t < incomingpayload.HitDistance) {
		payload.HitDistance = closest_t;
		payload.ObjectIndex = objectindex;
		payload.Polygonindex = polyonindex;
		//payload.WorldPosition = ray.orig + closest_t * ray.dir;
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