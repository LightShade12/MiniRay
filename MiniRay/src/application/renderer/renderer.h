#pragma once
#include "application/core/intern/image/image.h"
#include "application/camera/camera.h"
#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include "scene.h"
#include "UtilsCommon/RayTraceUtils.h"

class renderer
{
public:

	struct Settings
	{
		int MaxSamplesLimit = 100;
		int Bounces = 5;
		bool Accumulate = true;
		bool mt1997_Random = false;
	};

	renderer() = default;
	void OnResize(uint32_t width, uint32_t height);
	void render(const Scene& scene, const Camera& camera);
	std::shared_ptr<Image> GetFinalImage() const { return m_FinalImage; };
	Settings& GetSettings() { return m_Settings; };
	uint32_t GetSampleCount() const { return m_FrameIndex; };
	void ResetFrameIndex() { m_FrameIndex = 1; };
private:

	HitPayload TraceRay(const Ray& ray);
	glm::vec3 RayGen(uint32_t x, uint32_t y);//Raygen
	HitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);//closesthitshader
	HitPayload Miss(const Ray& ray);
	HitPayload Intersection(const Ray& ray, int objectindex, const HitPayload& incomingpayload);
	//MissShader

private:
	Settings m_Settings;
	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;
	uint32_t m_renderwidth, m_renderheight = 1;
	std::vector<glm::vec3>m_rawbuffer;
	std::vector<glm::vec3>m_accumulationbuffer;
	std::shared_ptr<Image>m_FinalImage;

	uint32_t m_FrameIndex = 1;
};
