#pragma once
#include "glm/glm.hpp"


struct Triangle
{
	Triangle() = default;

	Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal = { 0,0,0 }, glm::vec2 UVcoord0 = { 0,0 }, glm::vec2 UVcoord1 = { 0,0 }, glm::vec2 UVcoord2 = { 0,0 })
		:vert0(v0), vert1(v1), vert2(v2), Normal(normal) {
		//vert0 *= 100;
		//vert1 *= 100;
		//vert2 *= 100;

		//normals generation
		glm::vec3 edge1 = vert1 - vert0;
		glm::vec3 edge2 = vert2 - vert0;
		Normal = normalize(glm::cross(edge1, edge2));//swap edges for ccw or cw

		UVs[0] = UVcoord0;
		UVs[1] = UVcoord1;
		UVs[2] = UVcoord2;
	};

	glm::vec3 vert0 = { 1,1,1 };
	glm::vec3 vert1 = { 1,1,1 };
	glm::vec3 vert2 = { 1,1,1 };

	glm::vec3 Normal;
	glm::vec2 UVs[3];
	//std::string name = "triangle";//for testing only, remove it
	//int MaterialIndex = 0;

};


struct Ray {
	glm::vec3 orig = glm::vec3(0);
	glm::vec3 dir = glm::vec3(0);
};

struct HitPayload {
	HitPayload() = default;
	float HitDistance;
	glm::vec3 WorldPosition;//HitPoint
	glm::vec3 WorldNormal;
	int ModelIndex;
	int MeshIndex;
	Triangle triangle;//removes dependency for below
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
