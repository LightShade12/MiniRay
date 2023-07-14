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

struct Scene
{
	std::vector<Sphere> Spheres;
	std::vector<Material> Materials;
};