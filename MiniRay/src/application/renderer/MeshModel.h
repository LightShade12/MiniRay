#pragma once
#include "scene.h"
#include "glm/glm.hpp"
#include "UtilsCommon/RayTraceUtils.h"

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
	int MaterialIndex = 0;//may have multiple mats, just test with one
};

class MeshModel
{
public:
	MeshModel(std::string_view modelpath);
	MeshModel(Mesh mesh);//for testing
	std::vector<Mesh> m_Meshes;
	std::string name = "model";
	std::vector<Material> m_Materials;//unused
};