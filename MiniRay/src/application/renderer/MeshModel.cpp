#include "MeshModel.h"
#include <iostream>
#include "assimp/Importer.hpp"     // C++ importer interface
#include "assimp/scene.h"          // Output data structure
#include "assimp/postprocess.h"    // Post processing flags

Mesh::Mesh(const std::vector<float>& vertices)
{
	m_vertices = vertices;//do we need to do this?
	for (size_t i = 0; i < vertices.size(); i += 9)
	{
		glm::vec3 v0(vertices[i], vertices[i + 1], vertices[i + 2]);
		glm::vec3 v1(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
		glm::vec3 v2(vertices[i + 6], vertices[i + 7], vertices[i + 8]);

		m_triangles.push_back(Triangle(v0, v1, v2));
	}
}

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<int>& indices)
{
	m_vertices = vertices;//do we need to do this?

	std::vector<glm::vec3>tempvertices;//vector of coordinates of vertices

	//vertex creation
	for (size_t i = 0; i < vertices.size(); i += 3)
	{
		glm::vec3 vertex(vertices[i], vertices[i + 1], vertices[i + 2]);
		tempvertices.push_back(vertex);
	}

	//triangle assembly
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		m_triangles.push_back(Triangle(tempvertices[indices[i]], tempvertices[indices[i + 1]], tempvertices[indices[i + 2]]));
	}
}

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<int>& indices, const std::vector<float>& normals)
{
	m_vertices = vertices;//do we need to do this?

	std::vector<glm::vec3>tempvertices;//vector of coordinates of vertices
	std::vector<glm::vec3>tempnormals;//vector of normal

	//vertex creation
	for (size_t i = 0; i < vertices.size(); i += 3)
	{
		glm::vec3 vertex(vertices[i], vertices[i + 1], vertices[i + 2]);
		tempvertices.push_back(vertex);
	}
	for (size_t i = 0; i < normals.size(); i += 3)
	{
		glm::vec3 normal(normals[i], normals[i + 1], normals[i + 2]);
		tempnormals.push_back(normal);
	}

	//triangle assembly
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		m_triangles.push_back(Triangle(tempvertices[indices[i]], tempvertices[indices[i + 1]], tempvertices[indices[i + 2]], tempnormals[i]));
	}
}

//MeshModel::MeshModel(std::string_view modelpath){};

MeshModel::MeshModel(std::string_view modelpath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelpath.data(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	// If the import failed, report it
	if (nullptr == scene) {
		std::cerr << "import failed\n";
	}
	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		auto mesh = (scene->mMeshes[i]);

		std::vector<float> tempverts;
		std::vector<float> tempnrms;
		std::vector<int> indices;

		for (size_t j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
		{
			const aiVector3D* vert = &(mesh->mVertices[j]);
			tempverts.push_back(vert->x);
			tempverts.push_back(vert->y);
			tempverts.push_back(vert->z);
		}
		for (size_t k = 0; k < mesh->mNumFaces; k++) {
			aiFace face = mesh->mFaces[k];
			for (size_t l = 0; l < face.mNumIndices; l++) {
				size_t index = face.mIndices[l];
				indices.push_back(index);
			}
		}

		/*for (size_t l = 0; l < mesh->mNumVertices; l++)
		{
			const auto* normal = &(mesh->mNormals[l]);
			tempnrms.push_back(normal->x);
			tempnrms.push_back(normal->y);
			tempnrms.push_back(normal->z);
		}*/

		m_Meshes.push_back(Mesh(tempverts, indices));
	}
}

MeshModel::MeshModel(Mesh mesh)
{
	m_Meshes.push_back(mesh);
}