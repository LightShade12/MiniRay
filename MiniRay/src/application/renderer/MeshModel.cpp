#include "MeshModel.h"
#define CGLTF_IMPLEMENTATION
//#include "cgltf/cgltf.h"
#include <iostream>

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

//MeshModel::MeshModel(std::string_view modelpath)
//{
//	cgltf_options options;
//	memset(&options, 0, sizeof(cgltf_options));
//	cgltf_data* data = nullptr;
//	cgltf_result result = cgltf_parse_file(&options, modelpath.data(), &data);
//
//	if (result == cgltf_result_success)
//	{
//		result = cgltf_load_buffers(&options, data, modelpath.data());
//		if (result == cgltf_result_success)
//		{
//			// Successfully loaded glTF data, process it here
//			for (int i = 0; i < data->meshes_count; i++)
//			{
//				cgltf_mesh* mesh = &(data->meshes[i]);
//
//				const float* positions;
//				const uint32_t* indices;
//
//				for (size_t i = 0; i < mesh->primitives_count; ++i) 
//				{
//					cgltf_primitive* primitive = &(mesh->primitives[i]);
//
//					// Check if the primitive has positions (vertices)
//					if (primitive->type == cgltf_primitive_type_triangles && primitive->indices) 
//					{
//						positions = (float*)primitive->attributes[0].data;
//						//const float* positions = static_cast<float*>(primitive->attributes[0].data);
//						// Access the indices
//						indices = (uint32_t*)(primitive->indices);
//					}
//				}
//				
//
//				m_Meshes.push_back(Mesh(std::vector<float>(*positions), std::vector<int>(*indices)));
//			}
//		}
//	}
//	// After you finish using the data, free it
//	cgltf_free(data);
//}

MeshModel::MeshModel(Mesh mesh)
{
	m_Meshes.push_back(mesh);
}