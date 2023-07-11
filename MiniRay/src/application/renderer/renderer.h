#pragma once
#include "application/core/intern/image/image.h"
#include "application/camera/camera.h"
#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include "scene.h"

struct Ray {
	glm::vec3 orig = glm::vec3(0);
	glm::vec3 dir = glm::vec3(0);
};

struct HitPayload {
	float HitDistance;
	glm::vec3 WorldPosition;
	glm::vec3 WorldNormal;
	int ObjectIndex;
};

class renderer
{
public:

	struct Settings
	{
		int Bounces = 5;
		bool Accumulate = true;
		bool mt1997_Random = false;
	};

	renderer() = default;
	void OnResize(uint32_t width, uint32_t height);
	void render(const Scene& scene, const Camera& camera);
	std::shared_ptr<Image> GetFinalImage() const { return m_FinalImage; };
	Settings& GetSettings() { return m_Settings; };
	void ResetFrameIndex() { m_FrameIndex = 1; };
private:

	HitPayload TraceRay(const Ray& ray);
	glm::vec3 PerPixel(uint32_t x, uint32_t y);//Raygen
	HitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);//closesthitshader
	HitPayload Miss(const Ray& ray);//MissShader

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
