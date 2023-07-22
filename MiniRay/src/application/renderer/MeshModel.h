#pragma once
#include "scene.h"
#include "glm/glm.hpp"

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

	glm::vec3 vert0 = {1,1,1};
	glm::vec3 vert1 = { 1,1,1 };
	glm::vec3 vert2 = { 1,1,1 };

	glm::vec3 Normal;
	glm::vec2 UVs[3];
	//std::string name = "triangle";//for testing only, remove it
	//int MaterialIndex = 0;
};

//materials should also have images
//temporarily render meshes instead of models
class Mesh {
public:
	//only for testing
	Mesh(const std::vector<float>& vertices);//make non const when support for matrix transformations is available
	
	//standard
	Mesh(const std::vector<float>& vertices, const std::vector<int>& indices);
	Mesh(const std::vector<float>& vertices, const std::vector<int>& indices, const std::vector<float>& normals);
	std::vector<float>m_vertices;
	std::vector<int>m_indices;
	std::vector<Triangle>m_triangles;
	//std::string name = "mesh";
	//int MaterialIndex = 0;//may have multiple mats, just test with one
};

class MeshModel
{
public:
	MeshModel(std::string_view modelpath);
	MeshModel(Mesh mesh);//for testing
	std::vector<Mesh> m_Meshes;
	std::vector<Material> m_Materials;
	std::string name = "model";
	int MaterialIndex = 0;//may have multiple mats, just test with one
};