#pragma once
#include "glm/glm.hpp"

struct Triangle;

struct Ray {
	glm::vec3 orig = glm::vec3(0);
	glm::vec3 dir = glm::vec3(0);
};

struct HitPayload {
	HitPayload() = default;
	float HitDistance;
	glm::vec3 WorldPosition;//HitPoint
	glm::vec3 WorldNormal;
	int ObjectIndex;
	int MeshIndex;
	const Triangle* triangle_ptr=nullptr;//removes ddependency for below
	//int PolygonIndex;//used for triangleindexing
};

//dont call this manually!
namespace RayTraceIntern
{
	bool near_zero(glm::vec3 ray);

	static uint32_t PCG_Hash(uint32_t input) {
		uint32_t state = input * 747796405u + 2891336453u;
		uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
		return (word >> 22u) ^ word;
	};

	static float RandomFloat(uint32_t& seed)
	{
		seed = PCG_Hash(seed);
		return (float)seed / (float)std::numeric_limits<uint32_t>::max();

		//// Returns a random real in [0,1).
		//return rand() / (RAND_MAX + 1.0);
	};

	static glm::vec3 Vec3(uint32_t& seed, float min, float max)
	{
		return glm::vec3(RandomFloat(seed) * (max - min) + min, RandomFloat(seed) * (max - min) + min, RandomFloat(seed) * (max - min) + min);
	}

	//wrong implementation? Don't use
	static glm::vec3 InUnitSphere(uint32_t& seed)
	{
		return glm::normalize(glm::vec3(
			RandomFloat(seed) * 2.0f - 1.0f,
			RandomFloat(seed) * 2.0f - 1.0f,
			RandomFloat(seed) * 2.0f - 1.0f));

		/*while (true) {
			glm::vec3 p = glm::vec3(
				RandomFloat(seed) * 2.0f - 1.0f,
				RandomFloat(seed) * 2.0f - 1.0f,
				RandomFloat(seed) * 2.0f - 1.0f);
			if (glm::dot(p, p) >= 1) continue;
			return glm::normalize(p);
		}*/
	};
}
namespace RayTraceUtils
{
	inline glm::vec3 randomvec3(uint32_t& seed);

	glm::vec3 random_in_unit_sphere(uint32_t& seed);

	glm::vec3 random_unit_vector(uint32_t& seed);

	glm::vec3 lambertian_scatter(uint32_t& seed, HitPayload rec);
}
