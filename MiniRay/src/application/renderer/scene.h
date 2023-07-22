#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "MeshModel.h"

struct Material;
class Mesh;

struct Scene
{
	std::vector<Mesh> Meshes;
	std::vector<Material> Materials;
};