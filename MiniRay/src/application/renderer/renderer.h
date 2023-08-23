#pragma once
#include "application/core/intern/image/image.h"
#include "application/renderer/camera/camera.h"
#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include "scene.h"
#include "UtilsCommon/RayTraceUtils.h"
#include "application/renderer/bvh/bvh_node.h"

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
	void process(const Ray& ray, HitPayload& workingpayload, const std::shared_ptr<bvh_node>& node, trianglecluster& triclus, bool& leafcheck, bool& geomhit);
	void preorder(const Ray& ray, HitPayload& workingpayload, const std::shared_ptr<bvh_node>& root, trianglecluster& triclus, bool& leafcheck, bool& geomhit);

	HitPayload TraceRay(const Ray& ray);
	glm::vec3 RayGen(uint32_t x, uint32_t y);//Raygen
	HitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex, const Triangle& closestTriangle, int meshindex);
	//closesthitshader
	HitPayload Miss(const Ray& ray);
	HitPayload Intersection(const Ray& ray, int objectindex, const HitPayload& incomingpayload, const Triangle& triangle, int meshindex);
	//MissShader

private:
	std::vector<int>m_HorizontalIterator;
	std::vector<int>m_VerticalIterator;
	std::shared_ptr<bvh_node> m_Scenebvh;
	Settings m_Settings;
	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;
	uint32_t m_renderwidth, m_renderheight = 1;
	std::vector<glm::vec3>m_rawbuffer;
	std::vector<glm::vec3>m_accumulationbuffer;
	std::shared_ptr<Image>m_FinalImage;

	uint32_t m_FrameIndex = 1;
};
