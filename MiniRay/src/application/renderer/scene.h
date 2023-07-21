#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

struct Material
{
	glm::vec3 Albedo{ 1.0f };
	float Roughness = 1.0f;
	float Metallic = 0.0f;
	std::string name = "material";
	glm::vec3 EmissionColor{ 0 };
	float EmissionPower = 0;

	glm::vec3 GetEmmision() const { return EmissionColor * EmissionPower; };
};

struct Sphere
{
	glm::vec3 Position{0.0f};
	float Radius = 0.5f;
	std::string name = "sphere";
	int MaterialIndex = 0;
};

struct Triangle
{
	Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 normal = { 0,0,0 }, glm::vec2 UVcoord0 = { 0,0 }, glm::vec2 UVcoord1 = { 0,0 }, glm::vec2 UVcoord2 = { 0,0 })
		:vert0(v0), vert1(v1), vert2(v2), Normal(normal) {
		//vert0 *= 100;
		//vert1 *= 100;
		//vert2 *= 100;
		UVs[0] = UVcoord0;
		UVs[1] = UVcoord1;
		UVs[2] = UVcoord2;
	};

	glm::vec3 vert0;
	glm::vec3 vert1;
	glm::vec3 vert2;

	glm::vec3 Normal;
	glm::vec2 UVs[3];
	std::string name = "triangle";//for testing only, remove it
	int MaterialIndex = 0;
};

struct Scene
{
	std::vector<Triangle> Triangles;
	std::vector<Material> Materials;
};